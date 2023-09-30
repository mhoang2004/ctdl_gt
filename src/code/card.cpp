enum Suits {HEARTS = 3, DIAMONDS, CLUBS, SPADES};

class Card {
	private:
	    string ranks = ""; // 2->A
	    int suits; // clubs, diamonds, hearts, spades
	    int value;
	public:
		void setRanks(string ranks) {
	        this->ranks = ranks;
	    }
	    string getRanks() {
	        return ranks;
	    }
	    void setSuits(int suits) {
	        this->suits = suits;
	    }
	    int getSuits() {
	        return suits;
	    }
	    void setValue(string rank) {
	    	if(rank == "A") {
	    		this->value = 11;
	    	} else if(rank == "K" || rank == "Q" || rank == "J") {
	    		this->value = 10;
			} else {
				this->value = stoi(ranks);
			}
		}
	    int getValue() {
	    	return value;
		}
};

class PlayingCards {
	private:
	    static const string ranks[13];
	    static const int suits[4];
	    vector<Card> playingCards;
	public:
	    // Constructor
	    PlayingCards() {
	        this->createPlayingCards();
	    }
	    vector<Card> getPlayingCards() {
	        return playingCards;
	    }
	    void setPlayingCards(vector<Card> playingCards) {
	        this->playingCards = playingCards;
	    }
	    // Helper function to generate playingcards
	    void createPlayingCards() {
		    for(int j = 0; j < 13; j++) {
		        for(int k = 0; k < 4; k++) {
		            Card card;  // declare a new Card object inside the loop
		            card.setRanks(ranks[j]);
		            card.setSuits(suits[k]);
		            card.setValue(ranks[j]);
		            playingCards.push_back(card);
		        }
		    }
		}
	    void shufflePlayingCards() {
		    for(int i = 0; i < playingCards.size(); i++) {
		        int j = rand() % 52; // generate a random number between 0 - 51
		        Card temp = playingCards[i];
		        playingCards[i] = playingCards[j];
		        playingCards[j] = temp;
		    }
		}
		Card get1Card() {
		    Card lastCard = playingCards.back();
		    playingCards.pop_back();
		    return lastCard;
		}
};

const string PlayingCards::ranks[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
const int PlayingCards::suits[4] = {CLUBS, DIAMONDS, HEARTS, SPADES};