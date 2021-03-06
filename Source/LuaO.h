namespace LUA {
    namespace {
        std::map<int, WeakReference<LSelfKill>> objects;
        void store(int addr, WeakReference<LSelfKill> o);
        const char *SELF = "self";

        void reg(const LBase* key);
        void unreg(const LBase* key);
        bool set(const LBase* key, const char* name, int n_ = -1);
        void unset(const LBase* key, const char* name);
        bool hasCallback(const LBase* key, const char* name);

        void Set(lua_State *L_);
        void throwError(const char *msg);
        lua_State *Get();

        bool isEmpty(int i = -1);

        const var getNumber(int i= -1);
        const var checkAndGetNumber(int i=2, var def = 0);

        template<class T>
        const T getNumber(int i= -1);
        template<class T>
        const T checkAndGetNumber(int i=2, T def = 0);

        const bool getBoolean(int i = -1);
        const bool checkAndGetBoolean(int i=2, int def = false);

        const String getString(int i = -1);
        const String checkAndGetString(int i=2, String def = String::empty);

        const Array<var> getList(int i=-1);
            template<class T>
        const Array<T> getList(int i=-1);

        template<class T, class U = T>
        const Array<U*> getObjectList(int i = -1);
        const Array<Component*> getComponentList(int i=-1);

        const juce::Rectangle<int> getRectangle(int i=-1);
            template<class T>
        const juce::Rectangle<T> getRectangle(int i=-1);

        const juce::Line<int> getLine(int i=-1);
            template<class T>
        const juce::Line<T> getLine(int i=-1);

        const juce::Point<int> getPoint(int i=-1);
            template<class T>
        const juce::Point<T> getPoint(int i=-1);

        const juce::Range<int> getRange(int i=-1);
            template<class T>
        const juce::Range<T> getRange(int i=-1);

        const juce::SparseSet<int> getSparseSet(int i=-1);
        const std::list<var> getStdList(int i=-1);

            template<class T>
        T* toUserdata(int i=-1);

            template<class T>
        T* to_juce(int i=-1);

            template<class T>
        T* raw_cast(int i=-1);

            template<class T, class U = T>
        U* from_luce(int i=-1);

            template<class T, class U = T>
        int returnUserdata(const U* udata);
            template<class T, class U = T>
        int storeAndReturnUserdata(const U* udata);

        int returnBoolean(bool val);
        
        int returnNil();

            template<class T>
        int returnNumber(const T& val);

        const int returnString(const String& val);
        int returnString(const std::string& val);

        int TODO_OBJECT(const String& tmpl, const String& msg = "Not yet implemented: ");

        const int call_cb( const LBase* key, const char *name, int nb_ret = 0,
                                                                const std::list<var>& args = std::list<var>() );

        const String getError();

        // as for Component
        typedef int(*constructor_t)(Component* udata);
        typedef std::map<String, constructor_t> types_t;
        types_t types, testtypes;

        template<class T>
        int luacast(Component* udata) { return returnUserdata<T,Component>(udata); }

        template<class T>
        int testcast(Component* udata) {
            return( dynamic_cast<T*>(udata) != nullptr );
        }

        template<class T>
        void register_class(String const& n) {
            types.insert( std::make_pair(n, &luacast<T>));
            testtypes.insert( std::make_pair(n, &testcast<T>));
        }
        
        int casttype(String const& n, Component* udata) {
            types_t::iterator i = types.find(n);
            if( i==types.end() ) return 0;
            return i->second(udata);
        }

        int testtype(String const& n, Component* udata) {
            types_t::iterator i = testtypes.find(n);
            if( i==testtypes.end() ) return 0;
            return i->second(udata);
        }

        template<class T>
        T luaL_getnum(const char *t, int i = -1);
        const char* lua_getnumtype(int i = -1);

        // from LBase light user data
        typedef int(*constructor_unlight_t)(void* udata);
        typedef std::map<String, constructor_unlight_t> types_unlight_t;
        types_unlight_t types_unlight;

        template<class T, class U = T>
        int luacast_unlight(void* udata) {
            return returnUserdata<T,U>(static_cast<U*>(udata));
        }

        template<class T, class U = T>
        void register_class_unlight(String const& n) {
            types_unlight.insert( std::make_pair("L"+n, &luacast_unlight<T,U>) );
        }
        
        int casttype_unlight(String const& n, void* udata) {
            types_unlight_t::iterator i = types_unlight.find(n);
            if( i==types_unlight.end() ) return 0;
            return i->second(udata);
        }
    }
}

#define REGISTER_CLASS(T) LUA::register_class<T>(#T)
#define REGISTER_LIGHT_CLASS(T) LUA::register_class_unlight<L##T, T>(#T)
