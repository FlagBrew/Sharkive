# Sharkive

Welcome to **Sharkive** – your comprehensive Gateshark cheat code database designed for **Luma3DS 9.1+** and **Atmosphere 0.8.5+**!

## Overview

Sharkive serves as a community-driven database for cheat codes. While we strive to keep our database updated, many cheats are still unverified. Your contributions are invaluable!

### How You Can Help

- **Report Issues**: If you encounter cheats that don't work, please report them as issues in this repository. We will work to update or remove them to enhance the experience for all users.
- **Submit Cheats**: Have cheats that aren’t in Sharkive? Create a Pull Request to share your findings with the community!

## Frequently Asked Questions (FAQ)

### How do I install cheats on my console? Does Sharkive have an app?

**Sharkive** does not have a standalone app; instead, it is integrated seamlessly with [**Checkpoint**](https://github.com/FlagBrew/Checkpoint), a widely-used save manager for both 3DS and Switch consoles. You can follow the steps in [Updating Checkpoint's Cheat Database](#compile-db) to manually update Checkpoint's database to the latest commit.

### How do I use these cheats?

 Follow these steps to manage your cheats:

#### For 3DS via Luma:
1. Launch your game and open the Rosalina menu (`L + d-pad down + Select`).
2. Navigate to and select "Cheats".
3. Press `A` on the cheats you wish to activate.

#### For Switch via Atmosphère:
Unlike other Switch cheat engines/methodologies, Checkpoint only stores the cheats you selected to your console memory. That means cheats can only be managed inside Checkpoint. Here’s how to manage them:
1. Choose your desired cheats in Checkpoint.
2. Start the game associated with those cheats.
3. Verify that your cheats are active. If not, ensure that `atmosphere!dmnt_cheats_enabled_by_default` is enabled in your Atmosphère settings.

### Troubleshooting Cheats

If a cheat isn't working, consider the following:
1. Ensure you are using the latest version of your system's custom firmware (CFW):
   - **3DS**: [Luma3DS](https://github.com/AuroraWright/Luma3DS/releases/latest).
   - **Switch**: [Atmosphère](https://github.com/Atmosphere-NX/Atmosphere/releases/latest).
2. Cheats are often version-specific. Verify that the cheats match the game version you are playing.
3. If issues persist, please [report them](https://github.com/FlagBrew/Sharkive/issues) for further assistance.

### Requesting New Cheats

Nobody currently on the FlagBrew team makes cheats. All entries in Sharkive come from user submissions. If you have working cheats for a game, please submit a [pull request](https://github.com/FlagBrew/Sharkive/pulls) for inclusion in our database.

### Compatibility with DS and GBA Games

**Sharkive** does not support DS or GBA games. DS games operate on a separate firmware, making them incompatible with Luma's Rosalina and the cheat engine. For DS cheats, consider using [NitroHax](https://github.com/chishm/nitrohax).

### Understanding Cheat Indicators

Some cheats have a star (`*`) next to their name in Rosalina, indicating they require a key combo. If the cheat name doesn’t specify the combo, you can decode the lines starting with `DD000000` to find the necessary key combination. For assistance, use the bot on [our Discord server](https://discord.gg/bGKEyfY) in the #bot-channel with the `.cheatkeys` command.

### Viewing Full Cheat Names

If cheat names are truncated, this is due to Luma's limitations, not Sharkive. For a semi-complete list of titles in our database, check out [the wiki page](https://github.com/FlagBrew/Sharkive/wiki/3DS-games-in-the-database).

## <a name="compile-db"></a>Updating Checkpoint's Cheat Database

To update Checkpoint's cheat database, follow these steps:
1. Ensure you have [Python 3](https://www.python.org/downloads/) installed.
2. Clone or download the repository.
3. Open a command prompt/terminal in your repository folder.
4. Execute the command for your OS:
   - **Windows:** `py -3 joiner.py <console>`
   - **Mac / Linux:** `python3 joiner.py <console>`
5. Rename `<console>.json` to `cheats.json` and place it in the `/<console>/Checkpoint` folder on your SD card.

<sub>**Note**: `<console>` refers to your device type, either `3ds` or `switch`. To build databases for both, run the script without arguments.</sub>

## License

This project is licensed under the GNU GPLv3. For more details, see [LICENSE.md](https://github.com/FlagBrew/Sharkive/blob/master/LICENSE).

## Acknowledgments

- [JourneyOver](https://github.com/JourneyOver) for maintaining the cheat codes in Sharkive's database.
- All [cheat submitters](https://github.com/FlagBrew/Sharkive/graphs/contributors) and reporters.
- Contributors to [Luma3DS](https://github.com/AuroraWright/Luma3DS) and [Atmosphere](https://github.com/Atmosphere-NX/Atmosphere).

Thank you for being a part of the Sharkive community!
