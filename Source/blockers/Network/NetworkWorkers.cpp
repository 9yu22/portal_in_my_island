// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkWorkers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Private/BKChunkBase.h"
#include "HAL/UnrealMemory.h"
#include "blockers/blockersGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Protocol.h"
#include "ProcessQueue.h"
                  

//int8 recv_th_count = 0;

FRecvWorker::FRecvWorker(USGameInstance* Instance) : Instance(Instance)
{

}

FRecvWorker::~FRecvWorker()
{

}

void FRecvWorker::RecvPacket()
{

}

void FRecvWorker::MergePacket(uint8* buffer, int32 recvPacketSize) // 패킷 조립
{
    uint8* ptr = buffer;
    static size_t real_packet_size = 0;
    static size_t saved_packet_size = 0;
    static uint8 packet_buffer[BUFFER_SIZE];

    while (0 < recvPacketSize) {
        if (0 == real_packet_size) real_packet_size = ptr[0];

        // real_packet_size - saved_packet_size -> 받아온 데이터 중 이번 루프에서 처리된 데이터 크기

        if (recvPacketSize + saved_packet_size >= real_packet_size) { // 패킷 처리가 가능한 만큼 버퍼에 패킷이 있다면
            memcpy(packet_buffer + saved_packet_size, ptr, real_packet_size - saved_packet_size); // ptr[0] 만큼만(실제 처리할 패킷 크기만큼) 넘겨 처리
            ProcessPacket(packet_buffer);
            ptr += real_packet_size - saved_packet_size;
            recvPacketSize -= real_packet_size - saved_packet_size;
            real_packet_size = 0;
            saved_packet_size = 0;
        }
        else {
            memcpy(packet_buffer + saved_packet_size, ptr, recvPacketSize); // 버퍼에 데이터가 실제 패킷 크기보다 부족하다면 버퍼에 저장
            saved_packet_size += recvPacketSize;
            recvPacketSize = 0;
        }
    }
}

void FRecvWorker::ProcessPacket(uint8* packet)
{
    switch (packet[1]) {

    case SC_LOGIN_INFO: {
        SC_LOGIN_INFO_PACKET login;

        memcpy(&login, packet, sizeof(SC_LOGIN_INFO_PACKET));

        AsyncTask(ENamedThreads::GameThread, [this, login]()
            {
        if (IsValid(Instance->MyCharacter)) {
            Instance->MyCharacter->SetTextureForCharacter(login.id);
            Instance->MyCharacter->id = login.id;
            FVector NewLocation(login.x, login.y, login.z);
            Instance->MyCharacter->SetActorLocation(NewLocation);
            Instance->MyCharacter->IsSelf = true;
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Recv Login Packet id: %d, x: %f, y: %f, z: %f"), login.id, login.x, login.y, login.z));
            Instance->MyCharacter->loginOk = true;
        }
            });
        break;
    }

    case SC_MOVE_PLAYER: {
        SC_MOVE_PLAYER_PACKET new_pos;

        memcpy(&new_pos, packet, sizeof(new_pos));

        AsyncTask(ENamedThreads::GameThread, [this, new_pos]()
            {
                FVector NewLocation(new_pos.x, new_pos.y, new_pos.z);
                FRotator NewRotation(new_pos.pitch, new_pos.yaw, new_pos.roll);
                if (IsValid(Instance->MyCharacter)) {
                    for (auto& p : Instance->Players) {
                        if (p->id == new_pos.id && Instance->MyCharacter->id != new_pos.id) {
                            p->PacketLocation = NewLocation;
                            p->PacketRotation = NewRotation;
                            break;
                        }
                    }
                }
            });
      
        break;
    }
    case SC_ADD_PLAYER: {
        SC_ADD_PLAYER_PACKET new_player;

        memcpy(&new_player, packet, sizeof(new_player));

        AsyncTask(ENamedThreads::GameThread, [this, new_player]()
            {
                Instance->GameMode->SpawnCharacter(new_player);               
            });        
        
        break;
    }

    case SC_ADD_PORTAL: {
        SC_ADD_PORTAL_PACKET new_portal;

        memcpy(&new_portal, packet, sizeof(new_portal));

        AsyncTask(ENamedThreads::GameThread, [this, new_portal]()
            {
                Instance->GameMode->SpawnPortal(new_portal);
            });

        break;
    }

    case SC_ADD_BLOCK: {
        SC_ADD_BLOCK_PACKET add_block;

        memcpy(&add_block, packet, sizeof(add_block));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Recv Add Block Packet chunk index:%d, x: %d, y: %d, z: %d"), add_block.chunk_index, add_block.ix, add_block.iy, add_block.iz));

        //BlockInfo block;
        //block.chunk_index = add_block.chunk_index;
        //block.index = { add_block.ix, add_block.iy, add_block.iz };
        //block.type = static_cast<BKEBlock>(add_block.blocktype);

        AsyncTask(ENamedThreads::GameThread, [this, add_block]()
            {
                ABKChunkBase* chunk = Cast<ABKChunkBase>(Instance->ChunkWorld->Chunks[add_block.chunk_index]);
                chunk->AddBlock(add_block);
                //Instance->BlockQueue.Enqueue(block);
            });
       
        break;
    }

    case SC_REMOVE_BLOCK: {
        SC_REMOVE_BLOCK_PACKET remove_block;

        memcpy(&remove_block, packet, sizeof(remove_block));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Remove Block Packet chunk index:%d, x: %d, y: %d, z: %d, collapsible: %d"), remove_block.chunk_index, remove_block.ix, remove_block.iy, remove_block.iz, remove_block.collapsibleType));

        //BlockInfo block;
        //block.chunk_index = remove_block.chunk_index;
        //block.index = { remove_block.ix, remove_block.iy, remove_block.iz };
        //block.world_index = { remove_block.wx, remove_block.wy, remove_block.wz };
        //block.normal = { remove_block.nx, remove_block.ny, remove_block.nz };
        //block.type = static_cast<BKEBlock>(remove_block.blocktype);

        AsyncTask(ENamedThreads::GameThread, [this, remove_block]()
            {
                ABKChunkBase* chunk = Cast<ABKChunkBase>(Instance->ChunkWorld->Chunks[remove_block.chunk_index]); 
                chunk->RemoveBlock(remove_block);
                //Instance->BlockQueue.Enqueue(block);
            });

        break;
    }

    case ANIM: {
        ANIM_PACKET new_anim;
        memcpy(&new_anim, packet, sizeof(new_anim));

        AsyncTask(ENamedThreads::GameThread, [this, new_anim]()
            {
                for (auto p : Instance->Players) {
                    if (p->id == new_anim.id) {
                        p->SetMontage(static_cast<int32>(new_anim.anim_type));
                        break;
                    }
                }
            });
        break;
    }
    case SC_CHANGE_PLAYER_HP: {
        SC_CHANGE_PLAYER_HP_PACKET new_hp;

        memcpy(&new_hp, packet, sizeof(new_hp));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Recv New Player Hp Packet My Id:%d, Hp: %f"), new_hp.id, new_hp.hp));
        AsyncTask(ENamedThreads::GameThread, [this, new_hp]()
            {
                Instance->MyCharacter->health = new_hp.hp;
            });
    }            
        break;

    case SC_CHANGE_PORTAL_HP: {
        SC_CHANGE_PORTAL_HP_PACKET new_hp;

        memcpy(&new_hp, packet, sizeof(new_hp));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Recv New Portal Hp Packet My Id:%d, Hp: %f"), new_hp.id, new_hp.hp));
        AsyncTask(ENamedThreads::GameThread, [this, new_hp]()
            {
                for (auto p : Instance->Portals)
                    if (p->id == new_hp.id) {
                        p->Portalhealth = new_hp.hp;
                        break;
                    }                    
            });
        break;
    }

    case SC_ADD_ITEM: {
        SC_ADD_ITEM_PACKET add_item;

        memcpy(&add_item, packet, sizeof(add_item));
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Recv Add Item Packet x: %f, y: %f, z: %f"), add_item.x, add_item.y, add_item.z));
        UE_LOG(LogTemp, Warning, TEXT("Player%d Recv Add Item Packet x: %f, y: %f, z: %f / ItemType: %d"), Instance->MyCharacter->id, add_item.x, add_item.y, add_item.z, add_item.item_type);
        AsyncTask(ENamedThreads::GameThread, [this, add_item]()
            {
                Instance->GameMode->SpawnItem(add_item);
            });
        break;
    }

    //case SC_REMOVE_ITEM:{
    //    SC_ADD_ITEM_PACKET remove_item;

    //    memcpy(&remove_item, packet, sizeof(remove_item));

    //    AsyncTask(ENamedThreads::GameThread, [this, remove_item]()
    //        {
    //            for (auto& item : Instance->items) {
    //                if (remove_item.id == item->id) {
    //                    Instance->items_cs.Lock();
    //                    Instance->items.Remove(item);
    //                    Instance->items_cs.Unlock();
    //                    item->Destroy();
    //                    break;
    //                }
    //            }              
    //        });
    //    break;
    //}
    
    case SC_RESPAWN: {
        SC_RESPAWN_PACKET respawn;

        memcpy(&respawn, packet, sizeof(respawn));

        AsyncTask(ENamedThreads::GameThread, [this, respawn]()
            {
                for (auto& p : Instance->Players) {
                    if (p->id == respawn.id) {
                        p->health = respawn.hp + 1.0;
                        FVector respawn_location(respawn.x, respawn.y, respawn.z);
                        FRotator respawn_Rotation = FRotator::ZeroRotator;
                        p->SetActorLocation(respawn_location);
                        p->SetActorRotation(respawn_Rotation);
                    }
                }
            });
        break;
    }
        
    case SC_REMOVE_PLAYER: {
        SC_REMOVE_PLAYER_PACKET remove_player;
        memcpy(&remove_player, packet, sizeof(remove_player));

        AsyncTask(ENamedThreads::GameThread, [this, remove_player]()
            {
                for (auto& p : Instance->Players) {
                    if (p->id == remove_player.id) {
                        if (p->id == Instance->MyCharacter->id) {
                            Instance->DisconnectFromGameServer();                            
                        }
                        else { // 제거는 뮤텍스가 필요하지 않을까..
                            AblockersCharacter* remove_character = p;
                            Instance->Players.Remove(p);
                            remove_character->Destroy();

                            //// 포탈 제거(캐릭터에 포함되어 있으면 편한데..)
                            //for (auto& po : Instance->Portals) {
                            //    if (po->id == p->id) {
                            //        //APortal* remove_portal = po;
                            //        //Instance->Portals.Remove(po);
                            //        //remove_portal->Destroy();
                            //        //파괴하지 말고 숨기기만 하자. hp가 0이면 틱에서 숨겨지도록 되어 있다.
                            //        po->Portalhealth = 0.f;
                            //    }
                            //}                            
                        }
                    }
                }
            });
    }
    case SC_DESTROY_PORTAL: {
        SC_DESTROY_PORTAL_PACKET destroy_portal;
        memcpy(&destroy_portal, packet, sizeof(destroy_portal));

        AsyncTask(ENamedThreads::GameThread, [this, destroy_portal]()
            {
                for (auto& p : Instance->Portals) {
                    if (p->id == destroy_portal.id) {
                        p->Portalhealth = 0;
                    }
                }
            });
        break;
    }
    default:
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("<<<<Invalide Packet Buffer>>>>, Packet Type: %d"), packet[1]));
        break;
    }

    
}

bool FRecvWorker::Init()
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Thread Init")));
    return true;
}

uint32 FRecvWorker::Run()
{
    uint8 recv_buf[BUFFER_SIZE];

    //th_num = ++recv_th_count;

    while (recvRunning) {
        int32 BytesRead = 0;
        Instance->Socket->Recv(recv_buf, sizeof(recv_buf), BytesRead);
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Recv Data Size: %d"), BytesRead));
        UE_LOG(LogTemp, Display, TEXT("Player%d Recv Data Size: %d"),Instance->MyCharacter->id, BytesRead);
        if (BytesRead > 0) {
            MergePacket(recv_buf, BytesRead);

        } 

        //FPlatformProcess::Sleep(0.01);
    }
    return 0;
}

void FRecvWorker::Stop()
{
    recvRunning = false;
}

// SendWorker

//FSendWorker::FSendWorker(USGameInstance* Instance, AblockersCharacter* Character) : Instance(Instance), Character(Character)
//{
//    c_Socket = Instance->Socket;
//}
//
//FSendWorker::~FSendWorker()
//{
//}
//
//bool FSendWorker::Init()
//{
//    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Thread Init")));
//    return true;
//}
//
//uint32 FSendWorker::Run()
//{
//    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SendWorker Running Start")));
//    //UPacketHeader Header;
//
//    while (!Character->loginOk) {
//        // 로그인 패킷 받기 전까지 대기
//    }
//
//    int32 BytesSent = 0;
//       
//    double LastSendTime = FPlatformTime::Seconds();
//
//    //while (sendRunning)
//    //{
//    //    if (IsValid(Character))
//    //    {
//    //        CurrentLocation = Character->GetActorLocation();
//    //        CurrentRotation = Character->GetActorRotation();
//
//    //        double CurrentTime = FPlatformTime::Seconds();
//
//    //        if (CurrentTime - LastSendTime >= 0.1)
//    //        {
//    //            CS_MOVE_PACKET new_pos;
//    //            // 위치 및 회전 정보 패킷 구성
//    //            new_pos.type = CS_MOVE;
//    //            new_pos.size = sizeof(CS_MOVE_PACKET);
//    //            new_pos.x = CurrentLocation.X;
//    //            new_pos.y = CurrentLocation.Y;
//    //            new_pos.z = CurrentLocation.Z;
//    //            new_pos.pitch = CurrentRotation.Pitch;
//    //            new_pos.yaw = CurrentRotation.Yaw;
//    //            new_pos.roll = CurrentRotation.Roll;
//
//    //            BytesSent = 0;
//    //            c_Socket->Send((uint8*)&new_pos, sizeof(new_pos), BytesSent);
//
//    //            LastSendTime = CurrentTime;
//
//    //            // 디버그 메시지 출력
//    //            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Send My Move Packet, x: %f, y: %f, z: %f"), new_pos.x, new_pos.y, new_pos.z));
//    //        }
//    //    }
//    //    else
//    //    {
//    //        sendRunning = false;
//    //    }
//
//    //    //FPlatformProcess::Sleep(0.01);
//    //}
//
//    return 0;
//}
//
//void FSendWorker::Stop()
//{
//    sendRunning = false;
//}

