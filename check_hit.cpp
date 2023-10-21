#include <iostream>
#include <vector>
using namespace std;

enum Suits
{
	HEARTS = 3,
	DIAMONDS,
	CLUBS,
	SPADES
};
class card
{
private:
	int value;
	int suits;

public:
	int getValue()
	{
		return value;
	}
	int getSuits()
	{
		return suits;
	}
	void setValue(int value)
	{
		this->value = value;
	}
	void setSuits(int suits)
	{
		this->suits = suits;
	}
};
void sortCard(vector<card> selectedCards)
{
	int i, key, j;
	for (i = 1; i < selectedCards.size(); i++)
	{
		key = selectedCards[i].getValue();
		j = i - 1;
		while (j >= 0 && selectedCards[j].getValue() > key)
		{
			selectedCards[j + 1].setValue(selectedCards[j].getValue());
			j = j - 1;
		}
		selectedCards[j + 1].setValue(key);
	}
}
bool checkStraight(vector<card> selectedCards);
bool checkConsecutivePairs(vector<card> selectedCards)
{
	for (int i = 0; i < selectedCards.size(); i++)
	{
		if (selectedCards[i].getValue() == 15)
			return 0;
	}
	if (selectedCards.size() % 2 == 1)
		return 0;
	vector<card> straight_1;
	vector<card> straight_2;
	int number_1 = 0;
	int number_2 = 0;
	for (int i = 0; i < selectedCards.size(); i += 2)
	{
		straight_1[number_1] = selectedCards[i];
	}
	for (int i = 1; i < selectedCards.size(); i += 2)
	{
		straight_2[number_2] = selectedCards[i];
	}
	if (!checkStraight(straight_1))
		return 0;
	if (!checkStraight(straight_2))
		return 0;
	for (int i = 0; i < number_1; i++)
	{
		if (straight_1[i].getValue() != straight_2[i].getValue())
			return 0;
	}
	return 1;
}
bool checkStraight(vector<card> selectedCards)
{
	for (int i = 0; i < selectedCards.size(); i++)
	{
		if (selectedCards[i].getValue() == 15)
			return 0;
	}
	sortCard(selectedCards);
	for (int i = 0; i < selectedCards.size() - 1; i++)
	{
		if (selectedCards[selectedCards.size() - 1].getValue() != selectedCards[i].getValue() + selectedCards.size() - i - 1)
		{
			return 0;
		}
	}
	return 1;
}
bool checkPairs(vector<card> selectedCards)
{
	for (int i = 1; i < selectedCards.size(); i++)
		if (selectedCards[0].getValue() != selectedCards[i].getValue())
			return 0;
	return 1;
}
bool check(vector<card> selectedCards)
{
	if (selectedCards.size() == 1)
		return 1;
	if (selectedCards.size() >= 6)
		if (checkConsecutivePairs(selectedCards) == 1)
			return 1;
	if (selectedCards.size() > 2)
		if (checkStraight(selectedCards))
			return 1;
	if (selectedCards.size() >= 2)
		if (checkPairs(selectedCards))
			return 1;
	return 0;
}
int findValueMax(vector<card> selectedCards)
{
	int maxValue = selectedCards[0].getValue();
	for (int i = 1; i < selectedCards[0].getValue(); i++)
	{
		if (maxValue < selectedCards[i].getValue())
			maxValue = selectedCards[i].getValue();
	}
	return maxValue;
}

int findSuitsMax(vector<card> selectedCards)
{
	int index;
	for (int i = 0; i < selectedCards.size(); i++)
	{
		if (findValueMax(selectedCards) == selectedCards[i].getValue())
		{
			index = i;
			break;
		}
	}
	int maxSuits = selectedCards[index].getSuits();
	for (int i = 0; i < selectedCards.size(); i++)
	{
		if (findValueMax(selectedCards) == selectedCards[i].getValue() && selectedCards[i].getSuits() < maxSuits)
			maxSuits = selectedCards[i].getSuits();
	}
	return maxSuits;
}
card findMax(vector<card> selectedCards)
{
	card Cardreturn;
	Cardreturn.setSuits(findSuitsMax(selectedCards));
	Cardreturn.setValue(findValueMax(selectedCards));

	return Cardreturn;
}
bool check_hit(vector<card> history, vector<card> users)
{
	// check tu quy
	if (history.size() != users.size())
	{
		if (history.size() == 1 && history[0].getValue() == 15)
		{
			if (users.size() == 4 && checkPairs(users) == 1)
				return 1;
			if (users.size() >= 6 && checkConsecutivePairs(users) == 1)
				return 1;
		}
		if ((history.size() >= 6 && users.size() == 4) && checkConsecutivePairs(history) == 1 && checkPairs(history) == 1)
			return 1;
		if ((history.size() < users.size()) && checkConsecutivePairs(history) == 1 && checkConsecutivePairs(users) == 1)
			return 1;
	}

	if (history.size() == users.size())
	{
		// doi & tam & tu
		if (history.size() >= 2 || history.size() <= 4)
		{
			if (users.size() == history.size() && checkPairs(users) == 1 && checkPairs(history) == 1)
			{
				if (findValueMax(history) < findValueMax(users))
				{
					return 1;
				}
				else if (findValueMax(history) == findValueMax(users))

				{
					if (findSuitsMax(history) > findSuitsMax(users))
					{
						return 1;
					}
				}
			}
		}
		// sanh
		if (history.size() >= 3)
		{
			if (checkStraight(history) == 1 && checkStraight(users) == 1)
			{
				if (findValueMax(users) > findValueMax(history))
					return 1;
				else if (findValueMax(users) == findValueMax(history))
				{
					if (findSuitsMax(history) > findSuitsMax(users))
						return 1;
				}
			}
		}
		if (history.size() == 1)
		{
			if (history[0].getValue() < users[0].getValue())
				return 1;
			else if (history[0].getValue() == users[0].getValue())
			{
				if (history[0].getSuits() > users[0].getSuits())
					return 1;
			}
		}
	}
	return 0;
}
card *operator<<(ostream &os, card &x)
{
	switch (x.getValue())
	{
	case 11:
	{
		cout << "J";
		break;
	}
	case 12:
	{
		cout << "Q";
		break;
	}
	case 13:
	{
		cout << "K";
		break;
	}
	case 14:
	{
		cout << "A";
		break;
	}
	case 15:
	{
		cout << "2";
		break;
	}
	default:
	{
		cout << x.getValue();
		break;
	}
	}
	cout << "-";
	switch (x.getSuits())
	{
	case 3:
	{
		cout << "Co";
		break;
	}
	case 4:
	{
		cout << "Ro";
		break;
	}
	case 5:
	{
		cout << "Chuon";
		break;
	}
	case 6:
	{
		cout << "Bich";
		break;
	}
	}
}
int main()
{
	vector<card> history;
	card tmp;
	tmp.setValue(4);
	tmp.setSuits(5);
	tmp.setValue(4);
	tmp.setSuits(6);
	history.push_back(tmp);
	vector<card> users;
	tmp.setValue(4);
	tmp.setSuits(3);
	tmp.setValue(4);
	tmp.setSuits(4);
	users.push_back(tmp);
	cout << endl
		 << "Ket Qua: " << check_hit(history, users);
	return 0;
}
