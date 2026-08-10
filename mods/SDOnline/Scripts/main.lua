-- SDB CharCreation: hook the game's CharacterBarberMenu (natural OR programmatic).
-- Opens programmatically when the request-flag file exists.
-- Reads values from the widget's own properties (not just the controller).
-- Writes the done-file when menu closes.
--
-- File paths are suffixed with this process's PID (via the SDB_CC_PID env var,
-- set by the C++ mod in the same process) so two mod instances on one machine
-- don't race on the same request/done files — see mod.cpp's init_cc_ipc_paths().
local pidOk, pidVal = pcall(function() return os.getenv("SDB_CC_PID") end)
local PID_SUFFIX   = (pidOk and pidVal) or ""
local DONE_FILE    = "C:\\temp\\SDB_cc_done_" .. PID_SUFFIX .. ".json"
local REQUEST_FLAG = "C:\\temp\\SDB_cc_request_" .. PID_SUFFIX .. ".flag"
local WIDGET_PATH  = "/Game/UI/Widgets/Player/CharacterBarberMenu.CharacterBarberMenu_C"

if PID_SUFFIX == "" then
    print("[SDB-CC] WARNING: SDB_CC_PID not set (os.getenv unavailable or C++ mod not loaded yet) — "
        .. "falling back to unsuffixed IPC file names, which is not safe with a second instance "
        .. "on this machine\n")
end

-- All plausible property names the widget might expose for character data.
-- Covers: direct vars, text-box names, dropdown names, struct names.
local SCAN_PROPS = {
    "Forename","Surname","FirstName","LastName","CharacterName","DisplayName",
    "Sex","Gender","Age","Occupation",
    -- Text/EditBox widget child props (common UMG naming patterns)
    "EB_Forename","EB_Surname","EB_Age","EB_Sex","EB_Occupation",
    "TB_Forename","TB_Surname","TB_Age",
    "TxtForename","TxtSurname","TxtAge","TxtSex","TxtOcc",
    "Text_Forename","Text_Surname","Text_Age","Text_Sex","Text_Occupation",
    "Txt_Forename","Txt_Surname","Txt_Age","Txt_Sex","Txt_Occ",
    "Forename_TextBox","Surname_TextBox","Age_TextBox",
    "Input_Forename","Input_Surname","Input_Age",
    "Editable_Forename","Editable_Surname",
    -- Dropdown / combo box children
    "DropDown_Sex","DropDown_Age","DropDown_Occupation",
    "CB_Sex","CB_Occupation","CB_Age",
    "DropdownSex","DropdownAge","DropdownOccupation",
    -- Working state vars
    "CurrentForename","CurrentSurname","CurrentSex","CurrentAge",
    "SelectedSex","SelectedOcc","SelectedOccupation","SelectedAge",
    "CharacterData","CharData","Character","CharCreationData",
    -- Hair / appearance (useful to know for later)
    "Hair","Beard","Skin","Accessories","Eyebrows",
    "HairIndex","BeardIndex","SkinIndex",
    "SelectedHair","SelectedBeard","SelectedSkin",
    "CB_Hair","CB_Beard","CB_Skin",
}

-- ── helpers ───────────────────────────────────────────────────────────────────

local function tryStr(obj, name)
    local ok, v = pcall(function() return obj:GetPropertyValue(name) end)
    if not ok or v == nil then return nil end
    if type(v) == "string" then return v end
    local ok2, s = pcall(function() return v:ToString() end)
    return (ok2 and type(s)=="string") and s or tostring(v)
end

local function readStr(ctrl, name)
    return tryStr(ctrl, name) or ""
end

local function readOcc(ctrl)
    local ok, v = pcall(function() return ctrl:GetPropertyValue("Occupation") end)
    return ok and (tonumber(tostring(v)) or 0) or 0
end

-- Get a child UMG widget by name.
-- Tries GetWidgetFromName() first (proper UUserWidget API), then GetPropertyValue() fallback.
local function getChildWidget(parent, name)
    -- GetWidgetFromName is the correct UUserWidget API for named child widgets.
    local ok, child = pcall(function() return parent:GetWidgetFromName(name) end)
    if ok and child ~= nil then
        local okv = pcall(function() return child:IsValid() end)
        if okv then return child end
    end
    -- Fallback: blueprint variable reference.
    local ok2, child2 = pcall(function() return parent:GetPropertyValue(name) end)
    if ok2 and child2 ~= nil then return child2 end
    return nil
end

-- Read text from a child UMG widget (EditableTextBox, TextBlock, or ComboBoxString).
local function readChildText(parent, propName, verbose)
    local child = getChildWidget(parent, propName)
    if child == nil then
        if verbose then print("[SDB-CC]   "..propName..": child not found") end
        return ""
    end

    -- Method 1: GetText() — UTextBlock / UEditableText / UEditableTextBox
    local ok2, text = pcall(function() return child:GetText() end)
    if ok2 and text ~= nil then
        if type(text) == "string" then
            if verbose then print("[SDB-CC]   "..propName..": GetText()='"..text.."'") end
            return text
        end
        local ok3, s = pcall(function() return text:ToString() end)
        if ok3 and type(s) == "string" then
            if verbose then print("[SDB-CC]   "..propName..": GetText():ToString()='"..s.."'") end
            return s
        end
        if verbose then print("[SDB-CC]   "..propName..": GetText() returned "..type(text)..":"..tostring(text)) end
    else
        if verbose then print("[SDB-CC]   "..propName..": GetText() err="..tostring(text)) end
    end

    -- Method 2: "Text" UProperty directly (FText on UTextBlock/UEditableText)
    local ok4, textProp = pcall(function() return child:GetPropertyValue("Text") end)
    if ok4 and textProp ~= nil then
        if type(textProp) == "string" then
            if verbose then print("[SDB-CC]   "..propName..": .Text='"..textProp.."'") end
            return textProp
        end
        local ok5, s = pcall(function() return textProp:ToString() end)
        if ok5 and type(s) == "string" then
            if verbose then print("[SDB-CC]   "..propName..": .Text:ToString()='"..s.."'") end
            return s
        end
        if verbose then print("[SDB-CC]   "..propName..": .Text is "..type(textProp)..":"..tostring(textProp)) end
    else
        if verbose then print("[SDB-CC]   "..propName..": .Text err="..tostring(textProp)) end
    end

    -- Method 3: GetSelectedOption() — UComboBoxString
    local ok6, opt = pcall(function() return child:GetSelectedOption() end)
    if ok6 and type(opt) == "string" then
        if verbose then print("[SDB-CC]   "..propName..": GetSelectedOption()='"..opt.."'") end
        return opt
    else
        if verbose then print("[SDB-CC]   "..propName..": GetSelectedOption() err="..tostring(opt)) end
    end

    if verbose then print("[SDB-CC]   "..propName..": all methods failed") end
    return ""
end

-- Snapshot: text/occupation from controller; sex from our own keybind tracker.
local function ctrlSnapshot(ctrl)
    return {
        forename   = readStr(ctrl, "Forename"),
        surname    = readStr(ctrl, "Surname"),
        sex        = chosenSex,
        age        = readStr(ctrl, "Age"),
        occupation = readOcc(ctrl),
    }
end

local function jsonStr(s)
    s = tostring(s or ""):gsub('\\','\\\\'):gsub('"','\\"')
    return '"'..s..'"'
end

local function snapEqual(a, b)
    return a.forename==b.forename and a.surname==b.surname
       and a.sex==b.sex and a.age==b.age and a.occupation==b.occupation
end

local function writeDone(s)
    local json = '{"forename":'  .. jsonStr(s.forename)
              .. ',"surname":'   .. jsonStr(s.surname)
              .. ',"sex":'       .. jsonStr(s.sex)
              .. ',"age":'       .. jsonStr(s.age)
              .. ',"occupation":' .. tostring(s.occupation) .. '}'
    local f = io.open(DONE_FILE,"w")
    if f then f:write(json); f:close() end
    print("[SDB-CC] done → "..json)
end

local function fileExists(path)
    local f = io.open(path, "r")
    if f then f:close(); return true end
    return false
end

-- Dump every readable property from obj (widget or controller).
-- Returns a table of {name, value} for non-nil, non-empty hits.
local function dumpProps(obj, label)
    print("[SDB-CC] --- dumping "..label.." ---")
    local hits = {}
    for _, name in ipairs(SCAN_PROPS) do
        local v = tryStr(obj, name)
        if v ~= nil then
            local display = (v == "") and '""' or v
            print("[SDB-CC]   "..name.." = "..display)
            hits[name] = v
        end
    end
    print("[SDB-CC] --- end dump ---")
    return hits
end

-- ── per-menu tracking ─────────────────────────────────────────────────────────

local activeWidget   = nil
local lastSnap       = nil
local pollRunning    = false
local ccCompleted    = false
local chosenSex      = "Male"   -- M/F keys set this while menu is open

local function buildFinalSnapshot(ctrl)
    return ctrlSnapshot(ctrl)
end

local function stopTracking(ctrl, writeJson)
    if not pollRunning then return end
    pollRunning = false

    if writeJson then
        local s = buildFinalSnapshot(ctrl)
        print("[SDB-CC] menu closed — final:")
        print("  Forename="..s.forename.."  Surname="..s.surname
            .."  Sex="..s.sex.."  Age="..s.age.."  Occupation="..s.occupation)
        writeDone(s)
        ccCompleted = true
    end

    activeWidget = nil
    lastSnap     = nil
end

local function pollChanges()
    if not pollRunning then return end

    local gone = not activeWidget or not activeWidget:IsValid()
    if not gone then
        local ok, inVP = pcall(function() return activeWidget:IsInViewport() end)
        if ok and not inVP then gone = true end
    end

    local ctrl = FindFirstOf("BP_PlayerController_C")
    if gone then
        stopTracking(ctrl, true)
        return
    end

    if ctrl and ctrl:IsValid() then
        local s = ctrlSnapshot(ctrl)
        if lastSnap and not snapEqual(lastSnap, s) then
            print("[SDB-CC] CHANGED:"
                .." Forename="..s.forename
                .." Surname=" ..s.surname
                .." Sex="     ..s.sex
                .." Age="     ..s.age
                .." Occ="     ..s.occupation)
        end
        lastSnap = s
    end

    ExecuteWithDelay(300, pollChanges)
end

local function startTracking(widget)
    if pollRunning then return end
    activeWidget = widget
    pollRunning  = true
    ccCompleted  = false
    chosenSex    = "Male"   -- reset to default each time menu opens

    local ctrl = FindFirstOf("BP_PlayerController_C")
    if ctrl and ctrl:IsValid() then
        lastSnap = ctrlSnapshot(ctrl)
        print("[SDB-CC] menu opened — initial values:")
        print("  Forename="..lastSnap.forename.."  Surname="..lastSnap.surname
            .."  Sex="..lastSnap.sex.."  Age="..lastSnap.age
            .."  Occupation="..lastSnap.occupation)
    end

    ExecuteWithDelay(300, pollChanges)
    print("[SDB-CC] tracking started")
end

-- ── programmatic open ─────────────────────────────────────────────────────────

local function clearControllerFields(ctrl)
    for _, f in ipairs({"Forename","Surname","Age"}) do
        pcall(function() ctrl:SetPropertyValue(f, "") end)
    end
    -- Seed Sex with "Male" so the toggle has a non-blank starting value.
    pcall(function() ctrl:SetPropertyValue("Sex", "Male") end)
    pcall(function() ctrl:SetPropertyValue("Occupation", 0) end)
end

local function openCreationMenu()
    if pollRunning then
        print("[SDB-CC] openCreationMenu: already tracking a menu")
        return
    end

    local ctrl = FindFirstOf("BP_PlayerController_C")
    if not ctrl or not ctrl:IsValid() then
        print("[SDB-CC] openCreationMenu: no player controller")
        return
    end

    clearControllerFields(ctrl)

    local cls = StaticFindObject(WIDGET_PATH)
    if not cls or not cls:IsValid() then
        print("[SDB-CC] openCreationMenu: class not found")
        return
    end

    local widget = StaticConstructObject(cls, ctrl)
    if not widget or not widget:IsValid() then
        print("[SDB-CC] openCreationMenu: StaticConstructObject failed")
        return
    end

    widget:AddToViewport(0)
    print("[SDB-CC] openCreationMenu: widget shown")
    startTracking(widget)
end

-- ── detect menu opening ───────────────────────────────────────────────────────

local ok1, err1 = pcall(function()
    NotifyOnNewObject(
        WIDGET_PATH,
        function(widget)
            if ccCompleted then return end
            print("[SDB-CC] NotifyOnNewObject fired: "..widget:GetFullName())
            startTracking(widget)
        end)
    print("[SDB-CC] NotifyOnNewObject registered")
end)
if not ok1 then print("[SDB-CC] NotifyOnNewObject ERROR: "..tostring(err1)) end

local function scanForMenu()
    if not pollRunning and not ccCompleted and fileExists(REQUEST_FLAG) then
        print("[SDB-CC] request flag found — opening creation menu")
        os.remove(REQUEST_FLAG)
        openCreationMenu()
    end

    if not pollRunning and not ccCompleted then
        local w = FindFirstOf("CharacterBarberMenu_C")
        if w and w:IsValid() then
            local ok, inVP = pcall(function() return w:IsInViewport() end)
            if ok and inVP then
                print("[SDB-CC] scan found live CharacterBarberMenu in viewport")
                startTracking(w)
            end
        end
    end

    ExecuteWithDelay(1000, scanForMenu)
end
ExecuteWithDelay(3000, scanForMenu)

-- F7 = open creation menu
RegisterKeyBind(Key.F7, function()
    print("[SDB-CC] F7: opening creation menu")
    ccCompleted = false
    openCreationMenu()
end)

-- F8 = attach to already-open menu
RegisterKeyBind(Key.F8, function()
    local w = FindFirstOf("CharacterBarberMenu_C")
    if w and w:IsValid() then
        print("[SDB-CC] F8: attaching to live menu")
        ccCompleted = false
        startTracking(w)
    else
        print("[SDB-CC] F8: no live CharacterBarberMenu found")
    end
end)

-- Scan an object's properties, printing anything that looks like a string/number value.
local function scanPrimProps(obj, label, names)
    print("[SDB-CC] --- "..label.." ---")
    for _, n in ipairs(names) do
        local ok, v = pcall(function() return obj:GetPropertyValue(n) end)
        if ok and v ~= nil then
            local sv
            if type(v) == "string" then
                sv = '"'..v..'"'
            else
                -- Try ToString() to unwrap FString/FText userdata
                local ok2, s = pcall(function() return v:ToString() end)
                if ok2 and type(s) == "string" then
                    sv = '"'..s..'"  (via ToString)'
                else
                    sv = tostring(v)
                    -- Skip bare UObject/UFunction/AActor pointers — not useful
                    if sv:match("^UObject:") or sv:match("^UFunction:")
                        or sv:match("^AActor:") then sv = nil end
                end
            end
            if sv then print("[SDB-CC]   "..n.." = "..sv) end
        end
    end
end

local SEX_NAMES = {
    "Sex","Gender","SexType","SexIndex","GenderId","GenderType",
    "IsMale","IsFemale","CharSex","PlayerSex","BodyType","SexEnum",
    "CharGender","AppearanceSex","bIsFemale","bFemale","CharacterSex",
    "SexString","GenderString","SexName","GenderName",
}

-- Dump every readable primitive from a component using a broad name sweep.
local BROAD_PROPS = {
    -- generic value holders
    "Value","CurrentValue","ComponentValue","Data","Current","Selected",
    "Type","TypeIndex","Index","EnumValue","IntValue","BoolValue","StringValue",
    -- sex/gender specific
    "Sex","Gender","SexType","GenderType","SexValue","GenderValue",
    "SexIndex","GenderIndex","SexString","GenderString","SexName","GenderName",
    "bFemale","bMale","bIsFemale","bIsMale","IsFemale","IsMale",
    "CharSex","CharGender","PlayerSex","PlayerGender","BodySex","BodyGender",
    "AppearanceSex","AppearanceGender","CharacterSex","CharacterGender",
    -- character data fields (in case CharacterData has everything)
    "Forename","Surname","Age","Occupation","Hair","Beard","Skin",
    "HairIndex","BeardIndex","SkinIndex","AccessoriesIndex","EyebrowsIndex",
}

local function dumpComp(comp, label)
    print("[SDB-CC] >>> "..label.." <<<")
    for _, n in ipairs(BROAD_PROPS) do
        local ok, v = pcall(function() return comp:GetPropertyValue(n) end)
        if ok and v ~= nil then
            local sv
            if type(v) == "string" then
                sv = '"'..v..'"'
            else
                local ok2, s = pcall(function() return v:ToString() end)
                if ok2 and type(s) == "string" then
                    sv = '"'..s..'"'
                else
                    sv = tostring(v)
                    if sv:match("^UObject:") or sv:match("^UFunction:") or sv:match("^AActor:") then sv = nil end
                end
            end
            if sv then print("[SDB-CC]   "..n.." = "..sv) end
        end
    end
end

-- F9 = show current snapshot state
RegisterKeyBind(Key.F9, function()
    local ctrl = FindFirstOf("BP_PlayerController_C")
    if ctrl and ctrl:IsValid() then
        local s = ctrlSnapshot(ctrl)
        print("[SDB-CC] F9 snapshot: Forename='"..s.forename.."' Surname='"..s.surname
            .."' Sex='"..s.sex.."' Age='"..s.age.."' Occ="..s.occupation)
    end
end)

-- M / F keys: choose sex while creation menu is open
RegisterKeyBind(Key.M, function()
    if not pollRunning then return end
    chosenSex = "Male"
    print("[SDB-CC] Sex → Male  (press F for Female)")
end)
RegisterKeyBind(Key.F, function()
    if not pollRunning then return end
    chosenSex = "Female"
    print("[SDB-CC] Sex → Female  (press M for Male)")
end)

print("[SDB-CC] loaded")
print("[SDB-CC] F7=open  F8=attach  F9=check sex")
