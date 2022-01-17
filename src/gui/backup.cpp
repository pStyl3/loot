/*  LOOT

    A load order optimisation tool for
    Morrowind, Oblivion, Skyrim, Skyrim Special Edition, Skyrim VR,
    Fallout 3, Fallout: New Vegas, Fallout 4 and Fallout 4 VR.

    Copyright (C) 2022    Oliver Hamlet

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

#include "gui/backup.h"

namespace loot {
void createBackup(const std::filesystem::path& sourceDir,
                  const std::filesystem::path& destDir) {
  auto logger = getLogger();
  if (logger) {
    logger->info("Creating backup of {} in {}",
                 sourceDir.u8string(),
                 destDir.u8string());
  }

  for (auto it = std::filesystem::recursive_directory_iterator(sourceDir);
       it != std::filesystem::recursive_directory_iterator();
       ++it) {
    auto path = it->path();
    auto filename = path.filename().u8string();

    if (filename == ".git" || (it.depth() == 0 && filename == "backups")) {
      // Don't recurse into .git folders or the root backups folder.
      if (logger) {
        logger->info("Not recursing into directory {} at depth {}",
                     filename,
                     it.depth());
      }
      it.disable_recursion_pending();
    }

    if (!it->is_regular_file() ||
        (it.depth() == 0 && filename == "LOOTDebugLog.txt")) {
      // Skip copying the debug log and anything that isn't a normal file.
      if (logger) {
        logger->info(
            "Skipping directory entry {} at depth {}", filename, it.depth());
      }
      continue;
    }

    auto destPath = destDir / path.lexically_relative(sourceDir);

    std::filesystem::create_directories(destPath.parent_path());

    std::filesystem::copy(path, destPath);
  }

  if (logger) {
    logger->info("Backup created at {}", destDir.u8string());
  }
}
}
