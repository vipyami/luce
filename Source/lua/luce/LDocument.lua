-- must go to luce once finalised

--[[----------------------------------------------------------------------------

LDocument.lua

This class wraps DocumentWindow to be portable accross supported platforms.

Usage:

    local dw = luce:Document(name)

The main usage remains the same as its JUCE equivalent.

However, it adds one new method:

    closeWindow(quit_if_last)

        closes the window and quits the application is this is the last remaining one,
        except on OS X, where it just closes the window, unless specified otherwise.

    @alias meta

    @author Christophe Berbizier (cberbizier@peersuasive.com)
    @license GPLv3
    @copyright 

(c) 2014, Peersuasive Technologies

------------------------------------------------------------------------------]]
local LDEBUG = _G.LDEBUG
local luce   = _G.Luce
local app    = _G.App
local log, logError = app and app.log, app and app.logError

local className = "LDocument"

local mt = {}
mt.__index = mt

local function new(name, ...)
    local LDEBUG = _G.LDEBUG
    local luce   = _G.Luce
    local app    = _G.App
    local log, logError, logDebug = app.log, app.logError, app.logDebug

    local name = name or className
    -- pre-declaration
    local self = {
        name          = name,
        setFullScreen = nil,
        setSize       = nil,
        setBounds     = nil,
        closeWindow   = nil,
    }

    ---
    -- LDocument Class initialisation
    ---
    local this   = luce:DocumentWindow(name)
    local bounds = {0,0,800,600}
    local size   = {800, 600}
    -- TODO: set bounds instead of size, getting current bounds if a Point
    --       is provided
    local function set_size(size)
        if not(this) then return end
        if app.os.ios or app.os.android then
            if not(this:isFullScreen()) then
                this:setFullScreen(true) -- or kiosk ?
            end
        else
            this:centreWithSize(size)
        end
    end
    function self:setFullScreen(state)
        if (app.os.ios or app.os.android) then
            return
        else
            if state == this:isKioskMode() then return end
            if("nil"==type(state))then state = not(this:isKioskMode()) end
            this:setKioskMode(state)
        end
    end

    function self:setSize(size)
        set_size(size)
    end
 
    function self:setBounds(b)
        bounds = b
        size = { b.w, b.h }
        set_size(size)
    end

    function self:closeWindow(quit_if_last)
        if(app.os.linux or app.os.win or quit_if_last)then
            this:closeWindow(true)
        else
            this:closeWindow(false)
        end
    end

    self.__self = this.__self
    return setmetatable(self, {
        __self  = this.__self,
        __index = this,
        __newindex = this,
        __tostring = function() return className.."("..name..")" end
    })
end

mt.__call = function(...)
    return setmetatable({}, {
        __tostring = function() return className end,
        __call = function(_, name, ...)
            return new(name,...)
        end
    })
end

return setmetatable(mt, mt)