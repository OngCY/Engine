#include "GameLogic.h"

bool GameLogic::Init(void)
{
	m_pActorFactory = CreateActorFactory();
	CreatePlayerActor("configuration\\components_TranslatePlayer.json", ACTORID::PLAYER);
	RegisterPlayerDelegates();

	return true;
}

void GameLogic::Cleanup(void)
{
	RemovePlayerDelegates();

	if (m_pActorFactory)
	{
		delete m_pActorFactory;
		m_pActorFactory = nullptr;
	}
}

/*******ACTORS**********/
StrongActorPtr_t GameLogic::CreatePlayerActor(std::string filePath, MyTypes::ActorId actorId)
{
	StrongActorPtr_t pActor = m_pActorFactory->CreateActor(filePath.c_str(), actorId);
	if (pActor)
	{
		m_actors.insert(std::make_pair(pActor->GetActorID(), pActor));
		return pActor;
	}

	return StrongActorPtr_t();
}

WeakActorPtr GameLogic::GetActor(MyTypes::ActorId id)
{
	ActorMap_t::iterator it = m_actors.find(id);

	if (it != m_actors.end())
		return it->second;

	return WeakActorPtr();
}

ActorFactory* GameLogic::CreateActorFactory(void)
{
	return new ActorFactory();
}

/*******EVENT DELEGATIONS**********/
void GameLogic::RegisterPlayerDelegates(void)
{
	//create a delegate for the call back and register the delegate with the event manager
	ServiceLocator::GetEventService()->VRegisterListener(fastdelegate::MakeDelegate(this, &GameLogic::OnEvtTranslatePlayer), Event_TranslatePlayer::sk_EventId);
}

void GameLogic::RemovePlayerDelegates(void)
{
	//deregister the delegate from the event manager
	ServiceLocator::GetEventService()->VRemoveListener(fastdelegate::MakeDelegate(this, &GameLogic::OnEvtTranslatePlayer), Event_TranslatePlayer::sk_EventId);
}

//call back method
void GameLogic::OnEvtTranslatePlayer(IEventPtr pEvent)
{
	std::shared_ptr<Event_TranslatePlayer> pEvtTranslatePlayer = std::static_pointer_cast<Event_TranslatePlayer>(pEvent);
	MyTypes::ActorId id = pEvtTranslatePlayer->GetActorId();

	//add code to call the actor and execute its component
	StrongActorPtr_t pActor = GetActor(id).lock();
	if (pActor)
	{
		std::shared_ptr<TranslateComponent> pTranslateComponent = pActor->GetComponent<TranslateComponent>(TranslateComponent::COMPONENT_ID).lock();
		pTranslateComponent->VApplyTranslation();
	}

	std::string eventLog("TranslatePlayer event received for actor ID: ");
	eventLog += std::to_string(id);
	ServiceLocator::GetLogService()->VGetLogger()->info(eventLog);
}