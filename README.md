# StrongHold Game
A turn-based kingdom management simulation game built in C++. Players take control of a medieval kingdom, making decisions across leadership, military, economy, diplomacy, and population to build a thriving (or surviving) empire.
---Featues---

Core Mechanics:

Turn-Based Simulation: Each turn advances the kingdom by a year, simulating population, economy, military, and social dynamics.
Resource Management: Manage and produce vital resources like food, wood, and iron through farms, forests, and mines.
Social Classes: Govern peasants, merchants, and nobles, each with tax contributions and stability effects.
Military System: Recruit, train, and manage infantry and cavalry. Military strength depends on morale, training, and supplies.
Leadership Styles: Choose between monarchs and elected leaders, each with unique political dynamics.
Random Events: Handle famines, plagues, and invasions that test your kingdom’s resilience.
Save/Load System: Save the game state and reload it anytime to continue your kingdom’s legacy.
Logging System: All in-game events are logged to game_logs.txt for review and debugging.
Multiplayer Expansion:
Multiple Kingdoms: Manage several kingdoms in one session via a World class.
Communication System: Send and receive text-based messages between kingdoms using the Message class.
Alliances and Treaties: Form, break, or betray alliances. Stability penalties for dishonor.
Trade and Smuggling: Legal and illegal trade systems with market risk and embargo mechanics.
Warfare and Betrayal: Declare war, initiate battles, and manage diplomatic reputation.
Map and Territory Control: Simple 5x5 grid map for territory claiming and movement.
Simplicity and Structure:
Uses arrays instead of STL containers like vector for simplicity.
Designed with modularity and clarity, making it easy to extend or refactor.
Exception handling via try-catch blocks with meaningful std::runtime_error messages.

---File Structure Overview---

main.cpp: Entry point. Handles main menu, game loop, user input.
Kingdom.cpp: Core class managing simulation state and subsystems (economy, military, population).
Economy.cpp: Manages finances: treasury, tax rate, inflation, corruption, and resource flow.
Resource.cpp: Abstract and concrete classes for Food, Wood, Iron resource systems.
Bank.cpp: Handles loan system: issuance, repayment, audits.
Loan.cpp: Represents individual loans with terms and payment tracking.
Population.cpp: Manages demographics, employment, revolt risk, and growth.
Leader.cpp: Leadership system: Monarchs vs Elected leaders.
ArmyUnit.cpp: Military system: Infantry and Cavalry unit types.
Event.cpp: Random events that impact stability and strategy.

--Multiplayer Support Files:--

World.cpp: Central multiplayer manager for multiple kingdoms.
Message.cpp: Communication system for inter-kingdom messages.
Alliance.cpp: Diplomatic alliance system.
TradeOffer.cpp: Trade system for legal exchanges.
SmuggleOffer.cpp: Handles illegal transactions and risk mechanics.
Market.cpp: Manages active trade offers and embargo rules.
Map.cpp: Grid-based kingdom positioning and territorial control.

---Saving and Logging---

Game states are stored in structured files for reloading.
Game actions and events are written to game_logs.txt.
Multiplayer interactions (messages, trades, battles) are also logged.

---Sample Gameplay Flow---
Start a new game or load a saved one.
Make leadership decisions and handle yearly events.
Allocate resources, manage taxes, and maintain order.
Respond to invasions, famines, or diplomatic messages.
Expand territory and dominate through strength or diplomacy.


