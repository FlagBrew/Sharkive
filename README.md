# Sharkive

Gateshark cheat code database for Luma3DS 9.1+ and Atmosphere 0.8.5+.

## Database

Sharkive relies on a manually populated database to download and apply new cheats. Most of them are not yet confirmed to work.

**We need your help**: report all the cheats that aren't working as an issue in this repository. We will update them or remove them to avoid further issues with other users. Additionally, if you have cheats that aren't available in Sharkive yet, please create a Pull Request.

## FAQ
**Q:** How do I install cheats on my console? Does Sharkive have an app?<br>
**A:** Sharkive's cheat codes are bundled in [**Checkpoint**](https://github.com/FlagBrew/Checkpoint), which is a popular save manager for the 3DS and the Switch consoles. Download Checkpoint to manage Sharkive cheat codes. You can follow the steps in [How do I update Checkpoint's cheat database?](#compile-db) to manually update Checkpoint's database to latest commits.

**Q:** How do I use these cheats?<br>
**3ds via Luma**:
1. Load up your game and open the Rosalina menu (`L + d-pad down + Select` is the default combination)
2. Select "Cheats"
3. Press `A` on each of the cheats you want to use in order to activate them.

**Switch via Atmosphère**:<br>
Unlike other Switch cheat engines/methodologies, Checkpoint only stores the cheats you selected to your console memory. That means cheats can only be managed inside Checkpoint.
1. Select the cheats you want to use in Checkpoint
2. Launch the game the cheats are for
3. Confirm your cheats are running. If not, make sure that `atmosphere!dmnt_cheats_enabled_by_default` is enabled in Atmosphère settings.

**Q:** Help! This cheat doesn't work.<br>
**Q:** Why does the name turn red when I try to use this cheat?<br>
**A:** There can be a few causes for cheats not working:<br>
1. Make sure you're using the latest version of your system's preferred custom firmware (CFW)
    - **3DS**: [Luma3DS](https://github.com/AuroraWright/Luma3DS/releases/latest).
    - **Switch**: [Atmosphère](https://github.com/Atmosphere-NX/Atmosphere/releases/latest).
2. Cheats are all made for a particular version (game update) and/or revision of a game and will likely not work on others. Make sure the cheat(s) you are trying to use are for the version and/or revision you are playing.
3. If after trying the above you still can't get the cheat(s) to work, please [report them](https://github.com/FlagBrew/Sharkive/issues/new) so we can either relabel them for their proper version/revision or remove them.

**Q:** Can you add cheats for this game?<br>
**A:** Nobody currently on the FlagBrew team makes cheats. Everything in Sharkive's database comes from user submissions. If you have some working cheats for your game and want them included in Sharkive, submit a [pull request](https://github.com/FlagBrew/Sharkive/pulls) and, provided they have been tested, someone will merge it.

**Q:** Does this work for DS or GBA games?<br>
**A:** No. DS games run in a separate firmware, so Luma's Rosalina (and by extension, the cheat engine) cannot interact with them. You can look into [NitroHax](https://github.com/chishm/nitrohax) for DS cheats.

**Q:** Some cheats have a star (`*`) next to their name in Rosalina. Why?<br>
**A:** This means that the cheat makes use of a key combo. If the cheat name doesn't mention the key combo you'll have to look up the code and translate the line(s) starting `DD000000` to figure out what the key combo is. You can use the bot on [our Discord server](https://discord.gg/bGKEyfY) in #bot-channel (via the `.cheatkeys` command) to do this if you would like. Please note that you will need to follow the same instructions in the rules for accessing our Community channels to be able to see any message history there.

**Q:** Some cheat names are cut off in the list. Can you make the full name display? Can I read the full name somewhere?<br>
**A:** Cheat names getting cut off is due to Luma, not Sharkive. Check [the wiki page](https://github.com/FlagBrew/Sharkive/wiki/3DS-games-in-the-database) for titles in the database to find easy links to the cheat files you want to see.


## <a name="compile-db"></a>How do I update Checkpoint's cheat database?
Follow these steps:<br>
1. Make sure you have [Python 3](https://www.python.org/downloads/) installed
2. `git clone` or download the repository
3. Open a command prompt/terminal to your copy of the repo
4. Run the command for your OS:
    - **Windows:** `py -3 joiner.py <console>`
    - **Mac / Linux:** `python3 joiner.py <console>`
5. Rename `<console>.json` to `cheats.json` and place on your SD card in the `/<console>/Checkpoint` folder<br>

<sub>**Note**: `<console>` refers to the console you're using, `3ds` or `switch`. Pass no arguments to the script to build both 3ds and Switch databases.</sub><br>

## License

This project is licensed under the GNU GPLv3. See [LICENSE.md](https://github.com/FlagBrew/Sharkive/blob/master/LICENSE) for details.

## Credits

JourneyOver for providing and maintaining cheat codes in Sharkive's database.

All the cheats submitters and reporters.

All Luma3DS contributors.

All Atmosphere contributors.
