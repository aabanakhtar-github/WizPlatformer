#include "ClientEventSystem.h"
#include "Components.h"
#include <cstdint>
#include <sstream> 
#include <iostream>

// TODO left: 
// - Client server view synchronization (Clients see same as server) / correction
// - Client inputs sent to server
// - Server response to client input
// - Interpolation (depends)

void ClientEventSystem::Init(ECS::Scene& pscene) 
{
    m_NetClient = NetClient(std::bind(&ClientEventSystem::OnRecievePacket, this, std::placeholders::_1)); 
    if (!m_NetClient.GetValid()) 
    {
        GlobalAppState::Get().SetAppState(AppState::AS_FAIL, "Cannot initialize network!"); 
    }

// TODO refactor into reusable function that allows game to persist across multiple servers
    m_NetClient.Connect("127.0.0.1", 7777, 3.0); 

    EventHandler::Get().BindEvent(SDL_KEYDOWN, [&](SDL_Event& e) -> void
    { 
        switch (e.key.keysym.sym) 
        {
            case SDLK_SPACE:
                break;
        }  
    });   

    EventHandler::Get().BindEvent(SDL_QUIT, [&](SDL_Event& e) -> void 
    {
        GlobalAppState::Get().SetAppState(AppState::AS_QUIT);
    });
}

#if false
void ClientEventSystem::Update(ECS::Scene& scene, float delta)
{
    auto& component = scene.GetComponent<PhysicsBodyComponent>(0);
    
    input_packet.DataLength = output.str().size() + 1; 
    input_packet.Data = output.str(); 

    EventHandler::Get().Update();

    if (m_NetClient.GetConnected()) 
    { 
        m_NetClient.UpdateNetwork();
        m_NetClient.SendPacket(input_packet, 0, false);  
    }
}
#endif


void ClientEventSystem::Update(ECS::Scene& scene, float delta) 
{
    EventHandler::Get().Update(); 

    std::uint16_t inputs = GetKeyboardBits(); 
    // Predict Client Movement
    PredictClientState(); 
    // Interp other members 
    InterpolateEntities(); 

    if (m_NetClient.GetConnected())
    {
        // TODO: Check whether this is sufficient for the 
        m_NetClient.UpdateNetwork();
        PacketData packet; 
        packet.Type = PT_GAME_UPDATE; 
        ClientUpdatePayload payload; 
        payload.InputBits = inputs;
        payload.RequestID = m_InputSequenceNumber++; 
        //packet.Data = PayloadFromString<ClientUpdatePayload>(payload); 
        packet.DataLength = packet.Data.size() + 1;
    }
}

void ClientEventSystem::OnRecievePacket(const PacketData& packet) 
{
    switch (packet.Type) 
    {
    case PT_GAME_UPDATE: 
        // resync / reconcile / interpolate
        ReconcileWithServer(); 
        break;     
    }
}

void ClientEventSystem::PredictClientState()
{
    
}

void ClientEventSystem::ReconcileWithServer()
{

}

std::uint16_t ClientEventSystem::GetKeyboardBits()
{
    const std::uint8_t* keyboard_state = SDL_GetKeyboardState(nullptr); 
    std::uint16_t bits = 0;
    bits |= (keyboard_state[SDL_SCANCODE_W]) << 15; 
    bits |= (keyboard_state[SDL_SCANCODE_A]) << 14; 
    bits |= (keyboard_state[SDL_SCANCODE_S]) << 13; 
    bits |= (keyboard_state[SDL_SCANCODE_D]) << 12; 
    return bits; 
}

void ClientEventSystem::InterpolateEntities()
{

}
