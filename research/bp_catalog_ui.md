# UI Blueprint Catalog (FModel export analysis)

Source: `Exports/SurrounDead/Content/UI/**`. Export is flags-only (no bytecode) — function bodies are unknown, only name/flags/params exist. 392 UI JSON files total; 98 contain at least one `"Type": "Function"` entry (i.e. have BP graph logic beyond widget-tree layout), the other 294 are pure widget-tree/texture/style assets with zero script logic and are skipped here (menu backgrounds, static icon widgets, style rows, etc.).

## Top-line findings

- **No RPCs anywhere in `Content/UI/`.** A grep across all 98 logic-bearing files for `FUNC_Net`/`NetServer`/`NetMulticast`/`NetClient` returned zero matches. Every UI widget is pure client-side presentation — no widget calls a server RPC directly or exposes a multicast entry point. This matches the project's established pattern (seen in `bp_catalog_ai_vehicles.md`) of gameplay/replication logic living in actor/component classes, with UI as a passive display layer that gets pushed data via `UpdateHealth`/`UpdateFuel`-style BlueprintCallable functions.
- **No Net/Replicated properties found** in any UI class either — consistent with UI widgets being purely local, non-replicated `UserWidget` instances (standard UE pattern; each client's widgets are independent).
- **Roughly a third of the file count is real logic** (98/392); the rest is decorative. Of those 98, a large fraction (~40 files) belong to the **HQUI_ProgressBars** third-party plugin (`HQUI_ProgressBars/**`) — a generic, reusable progress-bar/marquee/effect-layer UMG library with 50-130 functions per class, entirely cosmetic (fill color, gradient, marquee animation, effect overlays). This is infrastructure, not gameplay logic — skip for gameplay purposes but it's the widget base (`ProgressBarLinear_C`/`ProgressBarCircular_C`) that essentially all HUD bars (health, fuel, durability, hunger/thirst) are built on top of.
- **Gameplay-relevant read points worth flagging**: several widgets have `BlueprintPure`/`BlueprintCallable` "Get" functions that pull real player-state for display — `RespawnScreen_C`/`ScreenPermadeath_C`: `Get_DaysSurvived`, `Get_DistanceTravelled`, `Get_HumansKilled`, `Get_ZombiesKilled` (all cast to `BP_PlayerController_C` and read stat fields); `Journal_InfoUI_C`: `GetAnimalsKilled`, `GetBossZombiesKilled`, `GetHumansKilled`, `GetZombiesKilled`; `Journal_SkillsUI_C`: `Update_SkillPoints`. None of these are networked — they're local reads against the owning client's controller/game-instance state, called at widget construct/update time. Useful reference points if a proxy needs equivalent stat display.
- **Health/Fuel push pattern**: `BP_RepairWidget_C`/`BP_FuelWidget_C`/`AIHealthBarUI_C`/`BuildableObjectHealth_C` all expose an identical `UpdateHealth(Health, MaxHealth)` / `UpdateFuel(Fuel, MaxFuel)` `BlueprintCallable|BlueprintEvent` function taking raw doubles — confirms the owning actor/component pushes numeric values into the widget on change rather than the widget polling or binding to a replicated property directly. This matches the vehicle health/fuel component pattern documented in `bp_catalog_ai_vehicles.md`.
- **KeypadUI_C and LockpickUI_C are the most "gameplay-adjacent" of the interactive widgets** — full numeric keypad button grid and a skill-check lockpicking minigame (drag/timing logic, `GetLockpickAmount` reading player's lockpick item count via `BP_JigMultiplayer_C`/inventory container structs, `BreakLockpick` calling into `ServerFindThenConsumeByItemID`-style consumption). These read/consume inventory items but the actual consumption RPC lives in the inventory component (Jigsaw plugin), not the widget itself — the widget only triggers it.
- **SaveMenu_C / RespawnScreen_C are unexpectedly large** (50 and up to ~150 functions respectively counting inherited/bound-event wrappers) because they drive the EasyMultiSave (`EMSInfoSaveGame`) plugin directly from UI — `DeleteSlot`, `SaveGameToSlot`, `SearchSaves`, `SetCurrentSaveSlotByName` — save/load is initiated client-side from these widgets, not routed through a server-authoritative call, which is normal for a local single-player-style save system but worth noting if multiplayer save semantics ever need auditing.

## Notable classes (grouped, terse)

### HUD / stat-display widgets
- **AIHealthBarUI** (`Widgets/AI/AIHealthBarUI.json`) — floating health bar shown above AI/zombies. `UpdateHealth(Health, MaxHealth)` — BlueprintCallable, feeds a `ProgressBarLinear_C` and text. No net properties.
- **AIMarkerUI** (`Widgets/AI/AIMarkerUI.json`) — minimap marker icon over AI. Just `Construct` + ubergraph; icon/color set via ExposeOnSpawn properties, not runtime functions.
- **BuildableObjectHealth** (`Widgets/Buildable/BuildableObjectHealth.json`) — same `UpdateHealth` pattern for placed structures.
- **BP_RepairWidget / BP_FuelWidget** (`Widgets/Vehicle/`) — vehicle interaction-prompt widgets; `UpdateHealth`/`UpdateFuel` respectively, matching `VehicleHealthComponent`/`VehicleFuelComponent` from the AI/vehicle catalog.
- **DurabilityUI** (`Widgets/Other/Durability/DurabilityUI.json`) — item durability icon+circular bar; only `Construct`/`PreConstruct`, no explicit update function found (likely driven by widget binding rather than a push function) — purpose clear, wiring mechanism unclear.
- **Compass, TimeUI, SafeZoneUI, FloatingDamageNumbersUI** — small HUD elements, mostly `Construct`/ubergraph only, minimal or no custom functions; purpose clear from name, logic is trivial/cosmetic.
- **MedicalUI** (`Widgets/Medical/MedicalUI.json`) — bleed-screen-effect overlay with `SetupUI` and a "Shake" widget animation; likely triggered on player bleed/damage state, purpose inferred not confirmed.

### Menus / meta screens
- **RespawnScreen** (`Widgets/Player/RespawnScreen.json`, ~150 funcs incl. bound-event wrappers) — post-death respawn choice screen (random spawn / chosen spawn point / load save / quit). Embeds `SaveMenu_C`. Pulls `Get_DaysSurvived`/`Get_DistanceTravelled`/`Get_HumansKilled`/`Get_ZombiesKilled` (BlueprintPure) from the player controller for the death-stats display.
- **ScreenPermadeath** (`Widgets/Player/ScreenPermadeath.json`) — permadeath end screen; same stat-getter pattern, quit-only (no respawn options).
- **SaveMenu / SaveGameSlot / SaveGameThumbnail** (`Menus/SaveGame/`) — save/load UI directly driving EasyMultiSave plugin calls (`DeleteSlot`, `SaveGameToSlot`, `SearchSaves`, `SetCurrentSaveSlotByName`) client-side.
- **PauseScreen / QuitWidgetRespawn** (`Menus/PauseScreen/`) — pause menu; `SavePressed`, `RemoveSaveAndSuicideButton`, `BindReturnToPauseFromSave`.
- **MenuWidget / CharacterCreatorMenu** (`Menus/Menu/`) — main menu and character creation flow; large function counts (37 each) but appear to be primarily menu-navigation/UI-state logic, not deep gameplay reads.
- **DebugMenu** (`Widgets/Debug/DebugMenu.json`, 56 funcs) — developer/cheat menu: damage player, reduce hunger/thirst, spawn items, reset skills, change weather — direct gameplay-state manipulation buttons, clearly a dev tool not meant for release builds. Worth knowing it exists if it's ever accidentally reachable.
- **Difficulty_Other / Difficulty_Vehicles / DifficultySettingsMenu / DifficultySettingsPage / NewGameDifficultySelectionPage** — new-game difficulty configuration UI, includes "reset skill tree"/"reset passive skills" buttons.
- **VideoSettingsPage / SettingsMenu / SettingRow(Big/Small) / CategoryButton / EnhancedMappingOption** — standard settings screens, no gameplay reads beyond an auto-save checkbox.

### Journal / quest / skill UI
- **JournalUI** (`Widgets/Journal/JournalUI.json`) — journal shell/tab container (skills/quests/info tabs).
- **Journal_QuestsUI** (17 funcs) — active/completed quest list; `ForgetQuest`, `GetAllActiveQuests`, `GetAllCompletedQuests`, `Get_SaveEnabledIcon_ToolTipWidget`.
- **Journal_SkillsUI** (36 funcs) — skill tree display; `Update_SkillPoints`, plus a "reset" bound event.
- **Journal_InfoUI** (17 funcs) — stat/lore info tab; `GetAnimalsKilled`, `GetBossZombiesKilled`, `GetHumansKilled`, `GetZombiesKilled` — all BlueprintPure reads of kill-count stats.
- **Journal_QuestTask / Journal_QuestButtonUI / Journal_QuestBranchUI / Journal_QuestConfirmForget / Journal_SkillTreeTooltip / ResetSkillsSelect** — smaller sub-widgets of the above, mostly ubergraph + bound-event wrappers only.

### Interaction-prompt / minigame widgets
- **KeypadUI** (`Widgets/Other/KeypadUI.json`, 18 funcs) — numeric keypad for locked doors/safes; digit buttons 0-9 + cancel, likely composes a code string and calls an interface function on the target object (not visible in this flags-only export).
- **LockpickUI / LockWidget** (`Lockpick/`, 12 + few funcs) — lockpicking minigame: `BreakLockpick` (consumes a lockpick via `ServerFindThenConsumeByItemID_Result` — the actual server call lives outside this widget, presumably on the inventory/Jigsaw component), `GetLockpickAmount`/`GetLockpickAmountText` (BlueprintPure reads of player's lockpick count from Jigsaw inventory structs), `ExitText` (dynamic keybind display).
- **PlayerMarkerSelectionUI, RadioUI, TravelToLocUI, LetterUI, SleepingUI, VehicleVendorButton, EnterTeleporterWidgetTutorial** — assorted world-interaction prompt widgets (radio station picker, travel confirm, readable letter, sleep confirm, vehicle vendor buy button, tutorial popup). Each has a handful of functions, purpose clear from name and widget content, no deep gameplay logic beyond triggering the associated interaction.
- **CharacterBarberMenu** (33 funcs) — appearance customization (hair/barber) menu.

### Radial menus / misc UMG plugin
- **UMG_RadialMenu / UI_RadialMenu / UMG_BasicRadialIcon / IRadialSlot / IRadialInput / BP_RadialMenuHelpers** (`RadialMenu/`) — a generic radial-menu UMG plugin (used for build/quick-select wheels presumably); self-contained, no gameplay-specific logic visible at this layer.
- **StyledAutoSettings / StyledRadioButton / StyledSliderSetting** — reusable settings-widget style plugin components.
- **HQUI_ProgressBars/** (`ProgressBarLinear`, `ProgressBarCircular`, `WB_Base`, `WB_Container_*`, `WB_Effect*`, `WB_PB_*`, `BPi_ProgressBars`, `BP_Widget_Functions`, `BP_ProgressBar_Functions`) — third-party generic progress-bar plugin, ~40 of the 98 logic-bearing files. Entirely cosmetic (fill/gradient/marquee/effect-layer parameters). This is the base class nearly every HUD bar in the game (health, fuel, durability, hunger/thirst, XP) is built from — structurally important as a dependency but contains no gameplay logic itself.

## Net/Replicated properties
None found in any of the 98 UI classes with logic (confirmed via full-text scan for `Net`/`RepNotify` property flags). UI widgets are local, non-replicated `UserWidget` instances throughout.
