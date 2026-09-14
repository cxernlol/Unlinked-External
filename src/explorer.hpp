#pragma once

/**
 * @file explorer.hpp
 * @brief Unlinked External - In-game instance tree explorer, class search, and hierarchy manipulation.
 */

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <chrono>
#include <vector>
#include "explorer_icons.h"

enum class TreeIcon : int {
    Model,
    Workspace,
    Folder,
    Gui,
    Stats,
    Sound,
    Players,
    Script,
    LocalScript,
    Module,
    Part,
    Spawn,
    Camera,
    Humanoid,
    Hat,
    Accessory,
    Player,
    ReplicatedStorage,
    ReplicatedFirst,
    StarterGui,
    StarterPack,
    StarterPlayer,
    CoreGui,
    Chat,
    RunService
};

struct TreeNode {
    const char* name;
    const char* type;
    int parent;
    TreeIcon icon;
};

inline constexpr TreeNode TreeNodes[ ] = {
    { "Ugc", "DataModel", -1, TreeIcon::Model },
    { "Workspace", "Workspace", 0, TreeIcon::Workspace },
    { "Baseplate", "Part", 1, TreeIcon::Part },
    { "SpawnLocation", "SpawnLocation", 1, TreeIcon::Spawn },
    { "Camera", "Camera", 1, TreeIcon::Camera },
    { "Terrain", "Terrain", 1, TreeIcon::Part },
    { "LocalHandler", "LocalScript", 1, TreeIcon::LocalScript },
    { "Dummy", "Model", 1, TreeIcon::Model },
    { "Humanoid", "Humanoid", 7, TreeIcon::Humanoid },
    { "CoolHat", "Hat", 7, TreeIcon::Hat },
    { "Face", "Accessory", 7, TreeIcon::Accessory },
    { "ReplicatedStorage", "ReplicatedStorage", 0, TreeIcon::ReplicatedStorage },
    { "Shared", "Folder", 11, TreeIcon::Folder },
    { "Net", "ModuleScript", 12, TreeIcon::Module },
    { "Util", "ModuleScript", 12, TreeIcon::Module },
    { "ServerScriptService", "ServerScriptService", 0, TreeIcon::Folder },
    { "Game", "Script", 15, TreeIcon::Script },
    { "Combat", "Script", 15, TreeIcon::Script },
    { "StarterPlayer", "StarterPlayer", 0, TreeIcon::StarterPlayer },
    { "StarterPlayerScripts", "StarterPlayerScripts", 18, TreeIcon::Folder },
    { "Client", "LocalScript", 19, TreeIcon::LocalScript },
    { "StarterCharacterScripts", "StarterCharacterScripts", 18, TreeIcon::Folder },
    { "Animate", "LocalScript", 21, TreeIcon::LocalScript },
    { "Players", "Players", 0, TreeIcon::Players },
    { "Player1", "Player", 23, TreeIcon::Player },
    { "Lighting", "Lighting", 0, TreeIcon::Folder },
    { "SoundService", "SoundService", 0, TreeIcon::Sound },
    { "GuiService", "GuiService", 0, TreeIcon::Gui },
    { "StarterGui", "StarterGui", 0, TreeIcon::StarterGui },
    { "HUD", "ScreenGui", 28, TreeIcon::Gui },
    { "StarterPack", "StarterPack", 0, TreeIcon::StarterPack },
    { "CoreGui", "CoreGui", 0, TreeIcon::CoreGui },
    { "TextChatService", "TextChatService", 0, TreeIcon::Chat },
    { "Stats", "Stats", 0, TreeIcon::Stats },
    { "ReplicatedFirst", "ReplicatedFirst", 0, TreeIcon::ReplicatedFirst },
    { "RunService", "RunService", 0, TreeIcon::RunService }
};

inline constexpr int TreeNodeCount = ( int )( sizeof( TreeNodes ) / sizeof( TreeNodes[ 0 ] ) );
inline constexpr int TreeIconCount = ( int )TreeIcon::RunService + 1;

[[nodiscard]] inline bool PngLooksValid( const std::vector< uint8_t >& Bytes ) noexcept {
    static constexpr unsigned char Sig[ 8 ] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    static constexpr unsigned char End[ 8 ] = { 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82 };
    if ( Bytes.size( ) < 33 )
        return false;
    if ( memcmp( Bytes.data( ), Sig, 8 ) != 0 )
        return false;
    return memcmp( Bytes.data( ) + Bytes.size( ) - 8, End, 8 ) == 0;
}

[[nodiscard]] inline const std::vector< uint8_t >* IconPng( TreeIcon Icon ) noexcept {
    switch ( Icon ) {
    case TreeIcon::Model: return &ExplorerIcons::model_image_data;
    case TreeIcon::Workspace: return &ExplorerIcons::workspace_image_data;
    case TreeIcon::Folder: return &ExplorerIcons::folder_image_data;
    case TreeIcon::Gui: return &ExplorerIcons::gui_service_image_data;
    case TreeIcon::Stats: return &ExplorerIcons::stats_image_data;
    case TreeIcon::Sound: return &ExplorerIcons::sound_image_data;
    case TreeIcon::Players: return &ExplorerIcons::players_image_data;
    case TreeIcon::Script: return &ExplorerIcons::script_image_data;
    case TreeIcon::LocalScript: return &ExplorerIcons::local_script_image_data;
    case TreeIcon::Module: return &ExplorerIcons::module_script_image_data;
    case TreeIcon::Part: return &ExplorerIcons::part_image_data;
    case TreeIcon::Spawn: return &ExplorerIcons::spawn_location_image_data;
    case TreeIcon::Camera: return &ExplorerIcons::camera_image_data;
    case TreeIcon::Humanoid: return &ExplorerIcons::humanoid_image_data;
    case TreeIcon::Hat: return &ExplorerIcons::hat_image_data;
    case TreeIcon::Accessory: return &ExplorerIcons::accessory_image_data;
    case TreeIcon::Player: return &ExplorerIcons::player_image_data;
    case TreeIcon::ReplicatedStorage: return &ExplorerIcons::replicated_storage_image_data;
    case TreeIcon::ReplicatedFirst: return &ExplorerIcons::replicated_first_image_data;
    case TreeIcon::StarterGui: return &ExplorerIcons::starter_gui_image_data;
    case TreeIcon::StarterPack: return &ExplorerIcons::starter_pack_image_data;
    case TreeIcon::StarterPlayer: return &ExplorerIcons::starter_player_image_data;
    case TreeIcon::CoreGui: return &ExplorerIcons::core_gui_image_data;
    case TreeIcon::Chat: return &ExplorerIcons::chat_image_data;
    case TreeIcon::RunService: return &ExplorerIcons::run_service_image_data;
    }
    return nullptr;
}

#if __has_include(<ur/app.hpp>)
inline unsigned long long PngIcon( const std::vector< uint8_t >& Bytes ) {
    CGraphics* Gfx = ur::app::graphics( );
    std::vector< unsigned char > Pixels;
    int Width = 0;
    int Height = 0;
    int Decoded = 0;
    unsigned long long Handle = 0;
    const char* Stage = "empty";
    if ( !Gfx || Bytes.empty( ) ) {
        Stage = !Gfx ? "no-gfx" : "empty-bytes";
    } else if ( !Pictures->Decode( Bytes.data( ), Bytes.size( ), Pixels, Width, Height, 0 ) ) {
        Stage = "decode-fail";
    } else if ( Width <= 0 || Height <= 0 || Pixels.empty( ) ) {
        Stage = "bad-size";
        Decoded = 1;
    } else {
        Decoded = 1;
        Handle = Gfx->CreateImage( Pixels.data( ), Width, Height );
        Stage = Handle ? "ok" : "create-fail";
    }
    // #region agent log
    {
        FILE* f = nullptr;
        if ( fopen_s( &f, "C:\\Users\\User\\Desktop\\Codes\\C++\\Unlinked External\\debug-1fba0a.log", "ab" ) == 0 && f ) {
            unsigned long long ts = ( unsigned long long )std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now( ).time_since_epoch( ) ).count( );
            fprintf( f, "{\"sessionId\":\"1fba0a\",\"hypothesisId\":\"B\",\"location\":\"explorer.hpp:PngIcon\",\"message\":\"png-upload\",\"data\":{\"bytes\":%zu,\"gfx\":%d,\"decoded\":%d,\"w\":%d,\"h\":%d,\"handle\":%llu,\"stage\":\"%s\"},\"timestamp\":%llu}\n",
                Bytes.size( ), Gfx ? 1 : 0, Decoded, Width, Height, Handle, Stage, ts );
            fclose( f );
        }
    }
    // #endregion
    return Handle;
}

inline unsigned long long TreeGlyph( TreeIcon Icon ) {
    static unsigned long long Cache[ 32 ] = { };
    int Index = ( int )Icon;
    if ( Index < 0 || Index >= 32 )
        return 0;
    if ( Cache[ Index ] )
        return Cache[ Index ];

    const std::vector< uint8_t >* Bytes = IconPng( Icon );
    const int Valid = Bytes && PngLooksValid( *Bytes ) ? 1 : 0;
    // #region agent log
    {
        FILE* f = nullptr;
        if ( fopen_s( &f, "C:\\Users\\User\\Desktop\\Codes\\C++\\Unlinked External\\debug-1fba0a.log", "ab" ) == 0 && f ) {
            unsigned long long ts = ( unsigned long long )std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now( ).time_since_epoch( ) ).count( );
            fprintf( f, "{\"sessionId\":\"1fba0a\",\"hypothesisId\":\"A\",\"location\":\"explorer.hpp:TreeGlyph\",\"message\":\"icon-lookup\",\"data\":{\"index\":%d,\"hasBytes\":%d,\"pngValid\":%d,\"byteCount\":%zu},\"timestamp\":%llu}\n",
                Index, Bytes ? 1 : 0, Valid, Bytes ? Bytes->size( ) : 0, ts );
            fclose( f );
        }
    }
    // #endregion
    if ( !Bytes )
        return 0;
    Cache[ Index ] = PngIcon( *Bytes );
    return Cache[ Index ];
}
#endif

inline TreeIcon IconFor( const char* Klass ) {
    if ( !Klass || !Klass[ 0 ] )
        return TreeIcon::Folder;
    if ( !_stricmp( Klass, "Workspace" ) )
        return TreeIcon::Workspace;
    if ( !_stricmp( Klass, "Players" ) )
        return TreeIcon::Players;
    if ( !_stricmp( Klass, "Player" ) )
        return TreeIcon::Player;
    if ( !_stricmp( Klass, "Humanoid" ) )
        return TreeIcon::Humanoid;
    if ( !_stricmp( Klass, "Camera" ) )
        return TreeIcon::Camera;
    if ( !_stricmp( Klass, "Folder" ) )
        return TreeIcon::Folder;
    if ( !_stricmp( Klass, "Model" ) )
        return TreeIcon::Model;
    if ( !_stricmp( Klass, "SpawnLocation" ) )
        return TreeIcon::Spawn;
    if ( !_stricmp( Klass, "Hat" ) )
        return TreeIcon::Hat;
    if ( !_stricmp( Klass, "Accessory" ) || !_stricmp( Klass, "Accoutrement" ) )
        return TreeIcon::Accessory;
    if ( !_stricmp( Klass, "LocalScript" ) )
        return TreeIcon::LocalScript;
    if ( !_stricmp( Klass, "ModuleScript" ) )
        return TreeIcon::Module;
    if ( !_stricmp( Klass, "Script" ) )
        return TreeIcon::Script;
    if ( !_stricmp( Klass, "ReplicatedStorage" ) )
        return TreeIcon::ReplicatedStorage;
    if ( !_stricmp( Klass, "ReplicatedFirst" ) )
        return TreeIcon::ReplicatedFirst;
    if ( !_stricmp( Klass, "StarterGui" ) )
        return TreeIcon::StarterGui;
    if ( !_stricmp( Klass, "StarterPack" ) )
        return TreeIcon::StarterPack;
    if ( !_stricmp( Klass, "StarterPlayer" ) )
        return TreeIcon::StarterPlayer;
    if ( !_stricmp( Klass, "CoreGui" ) )
        return TreeIcon::CoreGui;
    if ( !_stricmp( Klass, "SoundService" ) || !_stricmp( Klass, "Sound" ) )
        return TreeIcon::Sound;
    if ( !_stricmp( Klass, "Stats" ) )
        return TreeIcon::Stats;
    if ( !_stricmp( Klass, "RunService" ) )
        return TreeIcon::RunService;
    if ( !_stricmp( Klass, "TextChatService" ) || !_stricmp( Klass, "Chat" ) )
        return TreeIcon::Chat;
    if ( !_stricmp( Klass, "GuiService" ) || strstr( Klass, "Gui" ) || strstr( Klass, "Frame" ) || strstr( Klass, "Label" ) || strstr( Klass, "Button" ) )
        return TreeIcon::Gui;
    if ( !_stricmp( Klass, "Part" ) || !_stricmp( Klass, "MeshPart" ) || !_stricmp( Klass, "WedgePart" )
        || !_stricmp( Klass, "CornerWedgePart" ) || !_stricmp( Klass, "TrussPart" ) || !_stricmp( Klass, "UnionOperation" )
        || !_stricmp( Klass, "Terrain" ) || strstr( Klass, "Part" ) )
        return TreeIcon::Part;
    return TreeIcon::Folder;
}

[[nodiscard]] inline int TreeChildCount( int Index ) noexcept {
    int Count = 0;
    for ( int Node = 0; Node < TreeNodeCount; Node++ ) {
        if ( TreeNodes[ Node ].parent == Index )
            Count++;
    }
    return Count;
}

[[nodiscard]] inline bool TreeHasKids( int Index ) noexcept {
    return TreeChildCount( Index ) > 0;
}

[[nodiscard]] inline int TreeDepth( int Index ) noexcept {
    if ( Index < 0 || Index >= TreeNodeCount )
        return -1;
    int Depth = 0;
    int Current = Index;
    while ( Current >= 0 && Current < TreeNodeCount && Depth < 32 ) {
        int Parent = TreeNodes[ Current ].parent;
        if ( Parent < 0 )
            break;
        Depth++;
        Current = Parent;
    }
    return Depth;
}

[[nodiscard]] inline int FindTreeNode( const char* Name ) noexcept {
    if ( !Name || !Name[ 0 ] )
        return -1;
    for ( int Index = 0; Index < TreeNodeCount; Index++ ) {
        if ( !_stricmp( TreeNodes[ Index ].name, Name ) )
            return Index;
    }
    return -1;
}

