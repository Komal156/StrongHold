#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
using namespace std;

// Forward declarations
class SocialClass;
class Population;
class ArmyUnit;
class Leader;
class Resource;
class Loan;
class Event;
class Economy;
class Bank;
class Message;
class Alliance;
class TradeOffer;
class SmuggleOffer;
class Market;
class Map;
class World;

// Constants
const int MAX_SOCIAL_CLASSES = 10;
const int MAX_RESOURCES = 10;
const int MAX_KINGDOMS = 4;
const int MAX_MESSAGES = 50;
const int MAX_ALLIANCES = 20;
const int MAX_TRADES = 20;
const int MAP_SIZE = 5;

// Kingdom class
class Kingdom {
private:
    string name;
    int stability;
    int year;
    Population* population;
    SocialClass* socialClasses[MAX_SOCIAL_CLASSES];
    int socialClassCount;
    ArmyUnit* military;
    Leader* leader;
    Economy* economy;
    int mapX, mapY;
    int kingdomId;

public:
    Kingdom(const string& kingdomName, int id);
    virtual ~Kingdom();

    void simulateTurn();
    void recalculateStability();
    void adjustStability(int modifier);
    void displayStatus() const;
    void saveGameState(const string& filename) const;
    void loadGameState(const string& filename);
    void triggerRandomEvent();
    void resolveActiveEvents();
    void displayActiveEvents();
    void attack(Kingdom& defender);
    void betray(Kingdom& ally);
    void setMapPosition(int x, int y) { mapX = x; mapY = y; }

    string getName() const { return name; }
    int getStability() const { return stability; }
    int getYear() const { return year; }
    Population* getPopulation() const { return population; }
    ArmyUnit* getMilitary() const { return military; }
    Leader* getLeader() const { return leader; }
    Economy* getEconomy() const { return economy; }
    SocialClass** getSocialClasses() { return socialClasses; }
    int getSocialClassCount() const { return socialClassCount; }
    int getMapX() const { return mapX; }
    int getMapY() const { return mapY; }
    int getKingdomId() const { return kingdomId; }
};

// Message class
class Message {
private:
    string sender;
    string recipient;
    string content;
    int year;

public:
    Message(const string& sndr, const string& rcpt, const string& cntnt, int yr);
    void display() const;

    string getSender() const { return sender; }
    string getRecipient() const { return recipient; }
    string getContent() const { return content; }
    int getYear() const { return year; }
};

// Alliance class
class Alliance {
private:
    string kingdom1;
    string kingdom2;
    string type;
    bool active;
    int startYear;
    int duration;

public:
    Alliance(const string& k1, const string& k2, const string& typ, int yr, int dur);
    void breakAlliance();
    bool isActive() const { return active; }

    string getKingdom1() const { return kingdom1; }
    string getKingdom2() const { return kingdom2; }
    string getType() const { return type; }
    int getStartYear() const { return startYear; }
    int getDuration() const { return duration; }
};

// TradeOffer class
class TradeOffer {
private:
    string proposer;
    string target;
    string resourceType;
    int quantityOffered;
    string requestedResource;
    int quantityRequested;
    bool accepted;

public:
    TradeOffer(const string& prop, const string& tgt, const string& res, int qtyO, const string& reqRes, int qtyR);
    bool accept(Kingdom& proposerKingdom, Kingdom& targetKingdom);
    bool isValid() const { return quantityOffered > 0 && quantityRequested > 0; }

    string getProposer() const { return proposer; }
    string getTarget() const { return target; }
    string getResourceType() const { return resourceType; }
    int getQuantityOffered() const { return quantityOffered; }
    string getRequestedResource() const { return requestedResource; }
    int getQuantityRequested() const { return quantityRequested; }
    bool isAccepted() const { return accepted; }
};

// SmuggleOffer class
class SmuggleOffer : public TradeOffer {
private:
    int riskLevel;

public:
    SmuggleOffer(const string& prop, const string& tgt, const string& res, int qtyO, const string& reqRes, int qtyR, int risk);
    bool accept(Kingdom& proposerKingdom, Kingdom& targetKingdom);
    int getRiskLevel() const { return riskLevel; }
};

// Market class
class Market {
private:
    TradeOffer* trades[MAX_TRADES];
    SmuggleOffer* smuggles[MAX_TRADES];
    int tradeCount;
    int smuggleCount;

public:
    Market();
    ~Market();
    void addTrade(TradeOffer* trade);
    void addSmuggle(SmuggleOffer* smuggle);
    void processTrades(Kingdom** kingdoms, int kingdomCount);
    void displayTrades() const;
    void displaySmuggles() const;
};

// Map class
class Map {
private:
    int grid[MAP_SIZE][MAP_SIZE];
    string kingdomNames[MAX_KINGDOMS];
    int kingdomCount;

public:
    Map();
    void assignKingdom(const string& name, int x, int y, int id);
    void moveKingdom(int kingdomId, int newX, int newY);
    void display() const;
    int getDistance(const string& k1, const string& k2) const;
    bool isControlled(int x, int y, int kingdomId) const;
};
// World class (updated)
class World {
private:
    Kingdom* kingdoms[MAX_KINGDOMS];
    int kingdomCount;
    Message* messages[MAX_MESSAGES];
    int messageCount;
    Alliance* alliances[MAX_ALLIANCES];
    int allianceCount;
    Market* market;
    Map* map;
    int currentYear;

public:
    World();
    ~World();
    void addKingdom(Kingdom* kingdom);
    void simulateTurn();
    void sendMessage(const string& sender, const string& recipient, const string& content);
    void displayMessages(const string& kingdomName) const;
    void formAlliance(const string& k1, const string& k2, const string& type, int duration);
    void breakAlliance(const string& k1, const string& k2);
    void proposeTrade(const string& proposer, const string& target, const string& res, int qtyO, const string& reqRes, int qtyR);
    void proposeSmuggle(const string& proposer, const string& target, const string& res, int qtyO, const string& reqRes, int qtyR, int risk);
    void declareWar(const string& attacker, const string& defender);
    void moveKingdom(const string& kingdom, int x, int y);
    Kingdom* findKingdom(const string& name);
    bool areAllied(const string& k1, const string& k2) const;
    bool hasEmbargo(const string& k1, const string& k2) const;
    Kingdom** getKingdoms() const; // Remove inline implementation
    int getKingdomCount() const { return kingdomCount; }
    Market* getMarket() const { return market; }
    Map* getMap() const { return map; }
};

// Abstract SocialClass
class SocialClass {
protected:
    string className;
    int population;
    int happiness;
    int influence;

public:
    SocialClass(const string& name, int pop, int happy, int infl);
    virtual ~SocialClass() = default;

    virtual int calculateTaxContribution() const = 0;
    void updateHappiness(int modifier);
    virtual void demandChange() const;

    string getClassName() const { return className; }
    int getPopulation() const { return population; }
    int getHappiness() const { return happiness; }
    int getInfluence() const { return influence; }
    void setPopulation(int pop) { population = pop; }
};

// Derived Social Classes
class Peasant : public SocialClass {
private:
    int labor;
    int skill;

public:
    Peasant(int pop, int happy, int infl, int lab, int skl);
    int calculateTaxContribution() const override;
    void demandChange() const override;
};

class Merchant : public SocialClass {
private:
    int wealth;

public:
    Merchant(int pop, int happy, int infl, int wlth);
    int calculateTaxContribution() const override;
    void demandChange() const override;
};

class Noble : public SocialClass {
private:
    int land;

public:
    Noble(int pop, int happy, int infl, int lnd);
    int calculateTaxContribution() const override;
    void demandChange() const override;
};

// Population Dynamics
class Population {
private:
    int totalCount;
    int growthRate;
    int health;
    int employmentRate;

public:
    Population(int initialCount);
    virtual ~Population() = default;

    void calculateGrowth(int foodAvailability, int housingQuality);
    void updateHealth(int sanitation, int medicalCare);
    int getRevoltRisk(int happinessAvg) const;
    void migrate(int amount);

    int getTotalCount() const { return totalCount; }
    int getGrowthRate() const { return growthRate; }
    int getHealth() const { return health; }
    int getEmploymentRate() const { return employmentRate; }
};

// Military System
class ArmyUnit {
protected:
    int soldierCount;
    int morale;
    int trainingLevel;
    int equipmentQuality;

public:
    ArmyUnit(int soldiers, int mrle, int training, int equipment);
    virtual ~ArmyUnit() = default;

    virtual void recruit(int amount);
    virtual void train(int intensity);
    virtual int calculateStrength() const;
    void paySalaries(int amount);
    void updateMorale(int modifier);

    int getSoldierCount() const { return soldierCount; }
    int getMorale() const { return morale; }
    int getTrainingLevel() const { return trainingLevel; }
    int getEquipmentQuality() const { return equipmentQuality; }
};

// Derived Army Units
class Infantry : public ArmyUnit {
private:
    int armor;
    int weapons;

public:
    Infantry(int soldiers, int mrle, int training, int equipment, int armr, int wpns);
    int calculateStrength() const override;
    void train(int intensity) override;
};

class Cavalry : public ArmyUnit {
private:
    int horses;

public:
    Cavalry(int soldiers, int mrle, int training, int equipment, int hrs);
    int calculateStrength() const override;
    void recruit(int amount) override;
};

// Leadership Mechanics
class Leader {
protected:
    string name;
    int popularity;
    int corruptionLevel;
    string leadershipStyle;

public:
    Leader(const string& nm, int pop, int corrupt, const string& style);
    virtual ~Leader() = default;

    virtual void makeDecision(int option) = 0;
    virtual void addressCrisis();
    virtual bool holdElection() const;

    string getName() const { return name; }
    int getPopularity() const { return popularity; }
    int getCorruptionLevel() const { return corruptionLevel; }
    string getLeadershipStyle() const { return leadershipStyle; }
};

// Derived Leader Classes
class Monarch : public Leader {
private:
    string heir;
    int reignYears;

public:
    Monarch(const string& nm, int pop, int corrupt, const string& style, const string& hr, int years);
    void makeDecision(int option) override;
    bool holdElection() const override;
};

class ElectedLeader : public Leader {
private:
    int termLength;
    int currentTerm;

public:
    ElectedLeader(const string& nm, int pop, int corrupt, const string& style, int term, int current);
    void makeDecision(int option) override;
    bool holdElection() const override;
};

// Resource Management
class Resource {
protected:
    string name;
    int quantity;
    int productionRate;

public:
    Resource(const string& resName, int initialQuantity, int prodRate);
    virtual ~Resource() = default;

    virtual void produce(int efficiency);
    virtual bool consume(int amount);
    virtual int calculateDepletion() const;

    string getName() const { return name; }
    int getQuantity() const { return quantity; }
};

// Derived Resources
class Food : public Resource {
private:
    int farms;

public:
    Food(int initialQuantity, int farmCount);
    void produce(int efficiency) override;
    bool consume(int amount) override;
};

class Wood : public Resource {
private:
    int forests;

public:
    Wood(int initialQuantity, int forestCount);
    void produce(int efficiency) override;
};

class Iron : public Resource {
private:
    int mines;

public:
    Iron(int initialQuantity, int mineCount);
    void produce(int efficiency) override;
};

// Loan class
class Loan {
private:
    int amount;
    int interestRate;
    int dueDate;
    string lender;
    bool isPaid;

public:
    Loan(int amt, int rate, int due, const string& lndr);

    int calculateInterest() const;
    bool isOverdue(int currentYear) const;
    void makePayment(int payment);

    int getAmount() const { return amount; }
    int getInterestRate() const { return interestRate; }
    bool getIsPaid() const { return isPaid; }
    int getDueDate() const { return dueDate; }
    string getLender() const { return lender; }
};

// Bank System
class Bank {
private:
    int goldReserves;
    int loanInterestRate;
    int corruptionCases;
    Loan* activeLoans[MAX_RESOURCES];
    int loanCount;
    int year;

public:
    Bank(int initialReserves);

    Loan* issueLoan(int amount, int termYears, const string& lender);
    void collectDebt();
    void audit(int corruptionThreshold);
    bool detectFraud(int auditSeverity);

    int getGoldReserves() const { return goldReserves; }
    Loan** getActiveLoans() { return activeLoans; }
    int getLoanCount() const { return loanCount; }
};

// Event System
class Event {
protected:
    string name;
    int severity;
    int duration;
    int currentDuration;

public:
    Event(const string& evtName, int sev, int dur);
    virtual ~Event() = default;

    virtual void trigger(Kingdom& kingdom);
    virtual void resolve(Kingdom& kingdom);
    virtual bool isActive() const { return currentDuration < duration; }
    virtual int getImpact() const;

    string getName() const { return name; }
};

// Derived Events
class Famine : public Event {
private:
    int foodLossRate;

public:
    Famine(int severity);
    void trigger(Kingdom& kingdom) override;
    void resolve(Kingdom& kingdom) override;
    int getImpact() const override;
};

class Plague : public Event {
private:
    int mortalityRate;

public:
    Plague(int severity);
    void trigger(Kingdom& kingdom) override;
    void resolve(Kingdom& kingdom) override;
    int getImpact() const override;
};

class Invasion : public Event {
private:
    string enemy;
    int troopCount;

public:
    Invasion(int severity, const string& invadingEnemy);
    void trigger(Kingdom& kingdom) override;
    void resolve(Kingdom& kingdom) override;
    int getImpact() const override;
};

// Economy class
class Economy {
private:
    int treasury;
    int taxRate;
    int inflation;
    int gdp;
    int corruptionLevel;
    Bank* bank;
    Food food;
    Wood wood;
    Iron iron;

public:
    Economy(int initialTreasury);
    virtual ~Economy();

    void collectTaxes(SocialClass** socialClasses, int classCount);
    void addToTreasury(int amount);
    void adjustTaxRate(int newRate);
    void fundService(int amount);
    void calculateGDP(Population* population, SocialClass** socialClasses, int classCount);
    void updateInflation();
    void calculateInflation();
    void processLoans();
    int getTreasury() const { return treasury; }
    int getTaxRate() const { return taxRate; }
    int getInflation() const { return inflation; }
    int getGDP() const { return gdp; }
    int getCorruptionLevel() const { return corruptionLevel; }
    Bank* getBank() const { return bank; }
    Food* getFood() { return &food; }
    Wood* getWood() { return &wood; }
    Iron* getIron() { return &iron; }
};