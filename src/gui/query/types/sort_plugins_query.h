/*  LOOT

A load order optimisation tool for
Morrowind, Oblivion, Skyrim, Skyrim Special Edition, Skyrim VR,
Fallout 3, Fallout: New Vegas, Fallout 4 and Fallout 4 VR.

Copyright (C) 2014 WrinklyNinja

This file is part of LOOT.

LOOT is free software: you can redistribute
it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

LOOT is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LOOT.  If not, see
<https://www.gnu.org/licenses/>.
*/

#ifndef LOOT_GUI_QUERY_SORT_PLUGINS_QUERY
#define LOOT_GUI_QUERY_SORT_PLUGINS_QUERY

#include <boost/locale.hpp>

#include "gui/query/query.h"
#include "gui/state/game/game.h"
#include "gui/state/unapplied_change_counter.h"

namespace loot {
class SortPluginsQuery : public Query {
public:
  SortPluginsQuery(gui::Game& game,
                   UnappliedChangeCounter& counter,
                   std::string language,
                   std::function<void(std::string)> sendProgressUpdate) :
      game_(game),
      language_(language),
      counter_(counter),
      sendProgressUpdate_(sendProgressUpdate),
      useSortingErrorMessage(false) {}

  QueryResult executeLogic() override {
    auto logger = getLogger();
    if (logger) {
      logger->info("Beginning sorting operation.");
    }

    // Sort plugins into their load order.
    sendProgressUpdate_(boost::locale::translate("Sorting load order..."));
    std::vector<std::string> plugins = game_.SortPlugins();

    try {
      if (game_.GetSettings().Type() == GameType::tes5 ||
          game_.GetSettings().Type() == GameType::tes5se ||
          game_.GetSettings().Type() == GameType::tes5vr ||
          game_.GetSettings().Type() == GameType::fo4 ||
          game_.GetSettings().Type() == GameType::fo4vr)
        applyUnchangedLoadOrder(plugins);
    } catch (...) {
      useSortingErrorMessage = true;
      throw;
    }

    auto result = getResult(plugins);

    // plugins will be empty if there was a sorting error.
    if (!plugins.empty())
      counter_.IncrementUnappliedChangeCounter();

    return result;
  }

  std::string getErrorMessage() const override {
    if (useSortingErrorMessage) {
      return getSortingErrorMessage(game_);
    }

    return Query::getErrorMessage();
  }

private:
  void applyUnchangedLoadOrder(const std::vector<std::string>& plugins) {
    if (plugins.empty() ||
        !equal(begin(plugins), end(plugins), begin(game_.GetLoadOrder())))
      return;

    // Load order has not been changed, set it without asking for user input
    // because there are no changes to accept and some plugins' positions
    // may only be inferred and not written to loadorder.txt/plugins.txt.
    game_.SetLoadOrder(plugins);
  }

  std::vector<PluginItem> getResult(const std::vector<std::string>& plugins) {
    std::vector<PluginItem> result;

    for (const auto& pluginName : plugins) {
      auto plugin = game_.GetPlugin(pluginName);
      if (!plugin) {
        continue;
      }

      auto derivedMetadata = PluginItem(plugin, game_, language_);
      const auto index = game_.GetActiveLoadOrderIndex(*plugin, plugins);
      if (index.has_value()) {
        derivedMetadata.loadOrderIndex = index;
      }

      result.push_back(derivedMetadata);
    }

    return result;
  }

  gui::Game& game_;
  std::string language_;
  UnappliedChangeCounter& counter_;
  const std::function<void(std::string)> sendProgressUpdate_;
  bool useSortingErrorMessage;
};
}

#endif
