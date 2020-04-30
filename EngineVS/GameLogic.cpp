#include "GameLogic.h"

bool GameLogic::Init(void)
{
	m_pActorFactory = CreateActorFactory();
}

void GameLogic::Cleanup(void)
{
	if (m_pActorFactory)
	{
		delete m_pActorFactory;
		m_pActorFactory = nullptr;
	}
}

StrongActorPtr GameLogic::CreateActor(std::string filePath, MyTypes::ActorId actorId)
{
	StrongActorPtr pActor = m_pActorFactory->CreateActor(filePath.c_str(), actorId);
	if (pActor)
	{
		m_actors.insert(std::make_pair(pActor->GetActorID(), pActor));
		return pActor;
	}

	return StrongActorPtr();
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

//create a delegate for the call back and register the delegate with the event manager
void GameLogic::RegisterAllDelegates(void)
{
	//EventListenerDelegate delegateListener = fastdelegate::MakeDelegate(this, &GameLogic::OnEvtTransformActor);
	ServiceLocator::GetEventService()->VRegisterListener(fastdelegate::MakeDelegate(this, &GameLogic::OnEvtTransformActor), Event_TransformActor::sk_EventId);
}

void GameLogic::RemoveAllDelegates(void)
{
	//EventListenerDelegate delegateFunc = fastdelegate::MakeDelegate(this, &GameLogic::OnEvtTransformActor);
	ServiceLocator::GetEventService()->VRemoveListener(fastdelegate::MakeDelegate(this, &GameLogic::OnEvtTransformActor), Event_TransformActor::sk_EventId);
}

//call back method
void GameLogic::OnEvtTransformActor(IEventPtr pEvent)
{
	std::shared_ptr<Event_TransformActor> pEvtTransformActor = std::static_pointer_cast<Event_TransformActor>(pEvent);
	MyTypes::ActorId id = pEvtTransformActor->GetActorId();

	//add code to call the actor and execute its component
	StrongActorPtr pActor = GetActor(id).lock();
	if (pActor)
		std::shared_ptr<TransformComponent> pTransformComponent = pActor->GetComponent<TransformComponent>(TransformComponent::COMPONENT_ID).lock();
		//find the proper use of components

	std::string eventLog("Transform Actor event received for actor ID: ");
	eventLog += std::to_string(id);
	ServiceLocator::GetLogService()->VGetLogger()->info(eventLog);
}
