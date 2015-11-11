# RamAi (+ Nestopia)

RamAi is the implementation of a general game-playing AI that uses the RAM states of the game as input to the AI's value function; it accompanies my dissertation, "Reinforcement learning through evaluation of RAM states applied to a Nintendo game-playing AI" (working title) and will be completed by April 2016.

Nestopia is an open-source NES emulator that I have chosen to test the AI with. I have chosen the NES as a target platform due to its relatively small RAM size (2KiB) and input space (256 possible button combinations). Nestopia can be found here: http://nestopia.sourceforge.net/

Any changes that I have made to Nestopia are marked between [SLBEGIN] and [SLEND] comments.

The intention is that RamAI is reusable and portable between different emulators and games, but that's outside of the scope of the project for now. Hence, this repository will contain both the Nestopia source code (with my edits as needbe) and the RamAi source (all of which can be found in /RamAi/).

More details will be added later...