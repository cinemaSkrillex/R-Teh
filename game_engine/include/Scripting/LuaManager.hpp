#pragma once

#include <sol/sol.hpp>
#include <SFML/Graphics.hpp>

namespace RealEngine {
// class LuaManager {
//   public:
//     LuaManager();
//     ~LuaManager();
//     bool                       loadScript(const std::string& scriptPath);
// void                      addFunction(const std::string& name, sol::function function);
//     void                       unloadScript();
// void                     unloadFunction(const std::string& name);
// void emptyFunctions();
//     sol::table                 getGlobalTable();
//     sol::table                 getTable(const std::string& tableName);
//     sol::function              getFunction(const std::string& functionName);
//     template <typename T> void setGlobal(const std::string& name, T&& value) {
//         _luaState[name] = sol::make_object(_luaState, std::forward<T>(value));
//     }

//   private:
//     sol::state    _luaState;
// std::unordered_map<std::string, sol::function> _functions;
// };
} // namespace RealEngine