#include "i2-demo.h"

using namespace icinga;

string DemoComponent::GetName(void) const
{
	return "democomponent";
}

void DemoComponent::Start(void)
{
	m_DemoEndpoint = make_shared<VirtualEndpoint>();
	m_DemoEndpoint->RegisterMethodHandler("demo::HelloWorld",
	    bind_weak(&DemoComponent::HelloWorldRequestHandler, shared_from_this()));
	m_DemoEndpoint->RegisterMethodSource("demo::HelloWorld");

	EndpointManager::Ptr endpointManager = GetIcingaApplication()->GetEndpointManager();
	endpointManager->RegisterEndpoint(m_DemoEndpoint);

	m_DemoTimer = make_shared<Timer>();
	m_DemoTimer->SetInterval(5);
	m_DemoTimer->OnTimerExpired += bind_weak(&DemoComponent::DemoTimerHandler, shared_from_this());
	m_DemoTimer->Start();
}

void DemoComponent::Stop(void)
{
	IcingaApplication::Ptr app = GetIcingaApplication();

	if (app) {
		EndpointManager::Ptr endpointManager = app->GetEndpointManager();
		endpointManager->UnregisterEndpoint(m_DemoEndpoint);
	}
}

int DemoComponent::DemoTimerHandler(const TimerEventArgs& tea)
{
	Application::Log("Sending multicast 'hello world' message.");

	JsonRpcRequest request;
	request.SetMethod("demo::HelloWorld");

	EndpointManager::Ptr endpointManager = GetIcingaApplication()->GetEndpointManager();
	endpointManager->SendMulticastRequest(m_DemoEndpoint, request);

	return 0;
}

int DemoComponent::HelloWorldRequestHandler(const NewRequestEventArgs& nrea)
{
	Application::Log("Got 'hello world' from address=" + nrea.Sender->GetAddress() + ", identity=" + nrea.Sender->GetIdentity());

	return 0;
}

EXPORT_COMPONENT(DemoComponent);