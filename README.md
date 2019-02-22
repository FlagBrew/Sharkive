# Sharkive

Gateshark cheat code manager for Luma3DS 9.1+.

![](https://i.imgur.com/RYyVRJw.png)

## Usage

* 1: Run Sharkive.
* 2: You can download cheats for your games. (It's possible that some cheats don't work or crash while you try to use them).
* 3: Run your desired game.
* 4: Open Luma3DS's Rosalina menu.
* 5: Go to the `Cheats` section located on the top. Apply your cheats and return back to the game. You'll now be able to use your cheat.

## Database

Sharkive relies on a manually populated database to download and apply new cheats. Most of them are not yet confirmed to work.

**We need your help**: report all the cheats that aren't working as an issue in this repository. We will update them or remove them to avoid further issues with other users. Additionally, if you have cheats that aren't available in Sharkive yet, please create a Pull Request.

## FAQ
**Q:** Why won't my game show up in Sharkive?<br>
**A:** There are a couple reasons:<br>
1. Sharkive uses the same detection logic as Checkpoint: it only recognizes a title as a game if it finds a save in it -- make a save in the game and try running Sharkive again.
2. There is a bug in Sharkive's current title checking logic that causes it to sometimes miss a title even if there is a save. This seems to happen with games that were around for a previous check that didn't have a save before. You can force Sharkive to recheck all games by deleting `sd:/3ds/Sharkive/titles.sha` and running Sharkive again.

**Q:** Help! This cheat doesn't work.<br>
**Q:** Why does the name turn red when I try to use this cheat?<br>
**A:** There can be a few causes for cheats not working:<br>
1. There are bugs in the cheat engine of the latest release of Luma (as of this writing). A number of these are fixed in more recent nightly builds of Luma, which can be found [here](https://github.com/hax0kartik/luma-hourlies/releases/latest)
2. Cheats are all made for a particular version (game update) and/or revision of a game and will likely not work on others. Make sure the cheat(s) you are trying to use are for the version and/or revision you are playing.
3. Most of the cheats in Sharkive's database came from [JourneyOver's collection](https://github.com/JourneyOver/CTRPF-AR-CHEAT-CODES) which includes some Action Replay codes that aren't compatible with Luma's cheat engine.
4. If after trying the above you still can't get the cheat(s) to work, please [report them](https://github.com/FlagBrew/Sharkive/issues/new) so we can either relabel them for their proper version/revision or remove them.

**Q:** Can you add cheats for this game?<br>
**A:** Nobody currently on the FlagBrew team makes cheats. Everything in Sharkive's database comes from user submissions. If you have some working cheats for your game and want them included in Sharkive, submit a [pull request](https://github.com/FlagBrew/Sharkive/pulls) and someone will get around to adding them.

**Q:** Does this work for DS games?<br>
**A:** No. Luma's Rosalina cannot interact with DS games so its cheat engine is unavailable to them. You can try looking into [NitroHax](https://github.com/chishm/nitrohax) for DS cheats.

**Q:** Some cheats have a star (`*`) next to their name in Rosalina. Why?<br>
**A:** This means that the cheat makes use of a key combo. If the cheat name doesn't mention the key combo you'll have to look up the code and translate the line(s) starting `DD000000` to figure out what the key combo is.

**Q:** Some cheat names are cut off in the list. Can you make the full name display? Can I read the full name somewhere?<br>
**A:** Cheat names getting cut off is due to Luma, not Sharkive. The only thing we can potentially do is shorten the cheat's name. You can read the full cheat name by looking up the cheat file located either on your SD (see next question) or in [Sharkive's database](https://github.com/FlagBrew/Sharkive/tree/master/db) (you'll need your game's title ID in both cases).

**Q:** Where does Sharkive save the cheat files?<br>
**A:** `sd:/luma/titles/title_ID/cheats.txt` substituting `title_ID` with the title ID of the game you're looking for.


## Todo

Sharkive still relies on pp2d to render its graphics. I don't have enough time to take care of a Sharkive's citro2d port, so pull requests are highly appreciated.

Also, as you may notice, Sharkive is almost totally based on the legacy Checkpoint codebase (this software was originally written at very the beginning of 2018). That said, most of Checkpoint's optimizations are not yet included in Sharkive, so there will be updates in the future to make it run better. No promises though, as this project isn't very high priority for me.

## License

This project is licensed under the GNU GPLv3. See [LICENSE.md](https://github.com/FlagBrew/Sharkive/blob/master/LICENSE) for details.

## Credits

JourneyOver for providing and maintaining all the cheat codes in Sharkive's database.

All the cheats submitters and reporters.

All Luma3DS contributors.
