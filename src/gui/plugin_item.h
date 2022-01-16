/*  LOOT

    A load order optimisation tool for
    Morrowind, Oblivion, Skyrim, Skyrim Special Edition, Skyrim VR,
    Fallout 3, Fallout: New Vegas, Fallout 4 and Fallout 4 VR.

    Copyright (C) 2021    Oliver Hamlet

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

#ifndef LOOT_GUI_PLUGIN_ITEM
#define LOOT_GUI_PLUGIN_ITEM

#include <loot/metadata/group.h>
#include <loot/plugin_interface.h>
#include <loot/struct/simple_message.h>

#include <optional>
#include <string>

#include "gui/state/game/game.h"

namespace loot {
inline static const std::string DEFAULT_GROUP_NAME = Group().GetName();

struct PluginItem {
  PluginItem();
  PluginItem(const std::shared_ptr<const PluginInterface>& plugin,
             const gui::Game& game,
             std::string language);

  std::string name;
  std::optional<short> loadOrderIndex;
  std::optional<uint32_t> crc;
  std::optional<std::string> version;
  std::optional<std::string> group;
  std::optional<std::string> cleaningUtility;

  bool isActive;
  bool isDirty;
  bool isEmpty;
  bool isMaster;
  bool isLightPlugin;
  bool loadsArchive;
  bool hasUserMetadata;

  std::vector<std::string> currentTags;
  std::vector<std::string> addTags;
  std::vector<std::string> removeTags;

  std::vector<SimpleMessage> messages;

  bool containsText(const std::string& text) const;

  // QAbstractItemModel has a match() function that operates on items' strings,
  // so build a string that contains all the text that would be displayed for
  // the plugin's card.
  std::string contentToSearch() const;

  std::string getMarkdownContent() const;

  std::string loadOrderIndexText() const;
};
}

#endif