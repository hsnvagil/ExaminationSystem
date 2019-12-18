#include<iostream>
#include<fstream>
#include<conio.h>
#include<time.h>
#include<vector>
#include<string>
#include<iomanip>
#include<Windows.h>

using namespace std;
string uNa;

char* encrypt(char key[])
{
	for (int i = 0; i < strlen(key); i++)
	{
		if (i % 2 == 0)
			key[i] += (i + 1);
		else
			key[i] += (i + 2);
	}
	return key;
}
char* decryptkey(char code[])
{
	for (int i = 0; i < strlen(code); i++)
	{
		if (i % 2 == 0)
			code[i] -= (i + 1);
		else
			code[i] -= (i + 2);
	}
	return code;
}
void showConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(out, &cursorInfo);
}
class MyException
{
	string m_errMsg;
public:
	MyException()
	{
		m_errMsg = "\n\n\nERR::";
	}
	void setError(string str)
	{
		m_errMsg += str;
	}
	string getError() const
	{
		return m_errMsg;
	}
};
class Question
{
public:
	char m_qString[100];
	char m_optString[4][100];
	int m_level;
	int m_aIndex;
	void setIndex()
	{
		MyException ex1;
		try
		{
			cout << "\nEnter Answer Index (between 1 and 4)::";
			cin >> m_aIndex;
			m_aIndex--;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				ex1.setError("You can only enter numbers");
				throw ex1;
			}
			else if (m_aIndex < 0 || m_aIndex>3)
			{
				ex1.setError("Invalid answer index");
				throw ex1;
			}
		}
		catch (MyException ex)
		{
			cout << ex.getError();
			_getch();
			setIndex();
		}
	}
	void setLevel()
	{
		MyException ex1;
		try
		{
			cout << "\nEnter level number (between 0 and 9)::";
			cin >> m_level;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				ex1.setError("You can only enter numbers");
				throw ex1;
			}
			else if (m_level < 1 || m_level >9)
			{
				ex1.setError("Invalid level number");
				throw ex1;
			}
		}
		catch (MyException ex)
		{
			cout << ex.getError();
			_getch();
			setLevel();
		}
	}
	void viewQuestion()
	{
		cout << "\n\n\nLevel-" << m_level;
		cout << "\nQuestion String::" << m_qString;
		cout << "\nOptions::";
		for (int i = 0; i < 4; i++)
		{
			cout << "\n" << i + 1 << "::" << m_optString[i];
		}
		cout << "\nAnswer Index::" << m_aIndex + 1;
	}
	void newQuestion() {
		char temp;
		MyException ex;
		ofstream destFile;
		try
		{
			cout << "\nNote:Press ; to end";
			cout << "\nEnter Question::";
			cin.getline(m_qString, 100, ';');
			cout << "\nEnter Options::";
			for (int i = 0; i < 4; i++)
			{
				cout << "\nOption-" << i + 1 << "::";
				cin.getline(m_optString[i], 100, ';');
			}
			setIndex();
			setLevel();
			cout << "\nPress any key to review question.";
			_getch();
			viewQuestion();
			cout << "\n\nPress 1 to confirm question::";
			temp = _getche();
			if (temp == '1')
			{
				destFile.open("question.txt", ios::app | ios::binary);
				if (!destFile.is_open())
				{
					ex.setError("Cannot open question containing file");
					throw ex;
				}
				destFile.write((char*)this, sizeof(*this));
				if (!destFile.good())
				{
					ex.setError("Cannot write into file");
					throw ex;
				}
				cout << "\n\nYou have successfully added a question";
			}
		}
		catch (MyException ex)
		{
			cout << ex.getError();
			_getch();
		}
		destFile.close();
	}
	int questionSize()
	{
		ifstream fin;
		Question que;
		fin.open("question.txt", ios::binary);
		int size = 0;
		while (fin.good())
		{
			fin.read((char*)&que, sizeof(que));
			size++;
		}
		return size;
	}
};
class User
{
protected:
	char m_userName[20];
	char m_userPass[20];
	char m_name[20];
	int m_age;
	int m_userScore = 0;
	int m_posMarks = 0;
	int m_negMarks = 0;
public:
	const char* getUserName() const
	{
		return m_userName;
	}
	const char* getName() const
	{
		return m_name;
	}
	int getAge() const
	{
		return m_age;
	}
	int getScore() const
	{
		return m_userScore;
	}
	int getPosMark() const
	{
		return m_posMarks;
	}
	int getNegMark() const
	{
		return m_negMarks;
	}
	void setUserName(char u[])
	{
		strcpy_s(m_userName, u);
	}
	void setAge()
	{
		MyException ex1;
		try
		{
			cout << "\n\nAge::";
			cin >> m_age;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				ex1.setError("You can only enter numbers");
				throw ex1;
			}
			else if (m_age < 10 || m_age >50)
			{
				ex1.setError("Please enter appropriate age (Between 10 and 50)");
				throw ex1;
			}
		}
		catch (MyException ex)
		{
			cout << ex.getError();
			_getch();
			setAge();
		}
	}
	void startTest()
	{
		int qCounter = 0;
		Question currQue, que;
		try
		{
			if (que.questionSize() < 20)
			{
				MyException ex;
				ex.setError("Insufficient question in database, contact administrator");
				throw ex;
			}
		}
		catch (MyException ex)
		{
			cout << ex.getError();
			_getch();
			return;
		}
		User uObj;
		ifstream sourceFile;
		sourceFile.open(uNa, ios::in | ios::binary);
		sourceFile.read((char*)&uObj, sizeof(uObj));
		uObj.flushValues();
		vector<int> massiv;
		int userIndex;
		int randNum;
		time_t startTime, endTime;
		const int duration = 60 * 15;
		int elapseTime = 0;
		time(&startTime);

		while (qCounter < 20)
		{
			auto lambda = [&massiv, &randNum, &que]()
			{
				randNum = rand() % que.questionSize() + 1;
				for (int i = 0; i < massiv.size(); i++)
				{
					if (randNum == massiv[i])
						return false;
				}
				return true;
			};

			while (!lambda()) {}
			massiv.push_back(randNum);
			qCounter++;
			ifstream sourceFile;
			sourceFile.open("question.txt", ios::binary);
			for (int i = 0; i < randNum; i++)
			{
				sourceFile.read((char*)&currQue, sizeof(currQue));
			}

			time(&endTime);
			elapseTime = difftime(endTime, startTime);
			if (elapseTime >= duration)
			{
				cout << "\n\n\nYou have exceeded time limit.";
				Sleep(2000);
				cout << "\n\n\nPress any key to view results...";
				_getch();
				system("cls");
				break;
			}
			system("cls");
			cout << "\nTime elapsed::" << elapseTime / 60 << " Minute(s)\t\t\tYou are answering question " << qCounter << ".";
			cout << "\n\t     " << elapseTime % 60 << " Second(s)";
			cout << "\n\nQuestion::";
			cout << "  " << currQue.m_qString << endl;
			for (int i = 0; i < 4; i++)
			{
				cout << "\nOption-" << i + 1 << "::" << currQue.m_optString[i];
			}

			bool flag = true;
			while (flag)
			{
				cout << "\nPress Answer Index (between 1 and 4)::";
				char ch;
				ch = _getche();
				if (ch != '1' && ch != '2' && ch != '3' && ch != '4')
				{
					cout << "\nInvalid answer index";
				}
				else
				{
					flag = false;
					switch (ch)
					{
					case '1':
						userIndex = 0;
						break;
					case '2':
						userIndex = 1;
						break;
					case '3':
						userIndex = 2;
						break;
					case '4':
						userIndex = 3;
					}
				}

			}
			if (userIndex == currQue.m_aIndex)
			{
				uObj.m_userScore += currQue.m_level;
				uObj.m_posMarks++;
			}
			else
				uObj.m_negMarks++;
		}
		cout << "\n\nYour score::" << uObj.m_userScore;
		cout << "\nPositive marks::" << uObj.m_posMarks;
		cout << "\nNegative marks::" << uObj.m_negMarks;
		_getch();
		ofstream destFile;
		destFile.open(uNa, ios::binary | ios::trunc);
		destFile.write((char*)&uObj, sizeof(uObj));
		destFile.close();
	}
	void userHome()
	{
		system("cls");
		cout << "\n1 for Log In";
		cout << "\n(0 for Main Menu)\n";
		char ch;
		ch = _getche();
		if (ch == '1')
		{
			if (login())
			{
				do
				{
					system("cls");
					cout << "\n1 for Start Test";
					cout << "\n2 for Result Show";
					cout << "\n(0 to Logout)\n";
					ch = _getche();
					switch (ch)
					{
					case '1':
						startTest();
						break;
					case '2':
						resultShow();
						break;
					case '0':
						cout << "\n\n\nLogging out of profile";
						_getch();
						break;
					default:
						cout << "\n\n\nERR:Please enter appropriate choice.";
						_getch();
					}
				} while (ch != '0');
			}
		}
	}
	bool getPassword(string& strPassword)
	{
		strPassword.clear();
		int i = 0;
		char ch = '\0';
		cout << "\nPassword::";
		do
		{
			ch = _getch();
			if (ch == 13)
				break;
			else if (ch == 8)
			{
				i--;
				if (i >= 0)
				{
					cout << "\b \b";
					strPassword.pop_back();
				}
			}
			else
			{
				i++;
				cout << "*";
				strPassword.push_back(ch);
			}
		} while (true);
		try
		{
			MyException ex1;
			if (strPassword.empty())
			{
				ex1.setError("Password cannot be blank");
				throw ex1;
			}
			if (strPassword.length() < 5)
			{
				ex1.setError("Length of password must be greater than or equal to 5");
				throw ex1;
			}
			for (int i = 0; i < strPassword.length(); i++)
			{
				if (iscntrl(strPassword.at(i)) != 0)
				{
					ex1.setError("Password cannot contain control character");
					throw ex1;
				}
				if (ispunct(strPassword.at(i)) != 0)
				{
					ex1.setError("Password cannot contain punctuation character");
					throw ex1;
				}
				if (isspace(strPassword.at(i)) != 0)
				{
					ex1.setError("Password cannot contain space");
					throw ex1;
				}
			}
			return true;
		}
		catch (MyException ex)
		{
			cout << ex.getError();
			strPassword.clear();
			getPassword(strPassword);
		}
	}
	bool login()
	{
		User tempAdmin;
		string uTemp, pTemp;
		MyException ex1;
		system("cls");
		try
		{
			cout << "\n\n\nUser Name::";
			cin >> uTemp;
			uTemp += ".txt";
			ifstream userFile(uTemp, ios::binary | ios::in);
			if (!userFile.is_open())
			{
				ex1.setError("The username you've entered doesn’t match any account.");
				throw ex1;
			}
			uNa = uTemp;
			getPassword(pTemp);
			userFile.read((char*)&tempAdmin, sizeof(tempAdmin));
			if (strcmp(pTemp.c_str(), decryptkey(tempAdmin.m_userPass)) == 0)
			{
				userFile.close();
				return true;
			}
			else
			{
				ex1.setError("The password you've entered is incorrect.");
				throw ex1;
			}
			userFile.close();
			return false;
		}
		catch (MyException ex)
		{
			cout << ex.getError();
			_getch();
			return false;
		}
	}
	void resultShow()
	{
		ifstream sourceFile;
		sourceFile.open(uNa, ios::in | ios::binary);
		sourceFile.read((char*)this, sizeof(*this));
		decryptkey(m_userName);
		system("cls");
		cout << "\nUserName::" << m_userName;
		cout << "\nAge::" << m_age;
		cout << "\nUser Score::" << m_userScore;
		cout << "\nPositive Marks::" << m_posMarks;
		cout << "\nNegative Marks::" << m_negMarks;
		sourceFile.close();
		_getch();
	}
	void flushValues()
	{
		m_userScore = 0;
		m_posMarks = 0;
		m_negMarks = 0;
	}
};
class Admin :public User
{
private:
	char uName[20], password[20];
public:
	void displayUserList()
	{
		User uObj;
		system("cls");
		cout << "UserName" << setw(8) << ' ' << "Age" << setw(5)
			<< ' ' << "Score" << setw(5) << ' ' << "PosMarks" << setw(5) << ' ' << "NegMarks" << endl;

		ifstream sourceFile;

		sourceFile.open("users.txt", ios::binary | ios::in);
		string str;
		if (sourceFile.is_open())
		{
			while (getline(sourceFile, str))
			{
				ifstream tempFile;
				tempFile.open(str, ios::binary | ios::in);
				tempFile.read((char*)&uObj, sizeof(uObj));
				char temp[21];
				strncpy_s(temp, uObj.getUserName(), 21);
				decryptkey(temp);
				cout << temp << setw((18 - strlen(uObj.getUserName())))
					<< uObj.getAge() << setw(8)
					<< uObj.getScore() << setw(10)
					<< uObj.getPosMark() << setw(13)
					<< uObj.getNegMark() << endl;
				str.clear();
			}
		}
		sourceFile.close();
	}
	void viewQuestion()
	{
		ifstream sourceFile;
		MyException ex1;
		vector<Question>massiv;
		Question que;
		int qCounter;
		char temp = '\0';
		try
		{
			sourceFile.open("question.txt", ios::binary);
			if (!sourceFile.is_open())
			{
				ex1.setError("Cannot open question containing file");
				throw ex1;
			}
			qCounter = 0;

			while (sourceFile.good())
			{
				sourceFile.read((char*)&que, sizeof(que));
				massiv.push_back(que);
			}
			massiv.pop_back();

			while (true)
			{
				system("cls");
				cout << "Question number::" << qCounter + 1 << "\n";
				cout << "\n\n\nLevel-" << massiv[qCounter].m_level;
				cout << "\nQuestion String::" << massiv[qCounter].m_qString;
				cout << "\nOptions::";
				for (int i = 0; i < 4; i++)
				{
					cout << "\n" << i + 1 << "::" << massiv[qCounter].m_optString[i];
				}
				cout << "\nAnswer Index::" << massiv[qCounter].m_aIndex + 1;
				cout << endl;
				cout << "\n\nPress 0 to exit\n(Press 1 for Previous question or 2 for Next question)\n";
				temp = _getche();
				if (temp == '1' && qCounter != 0)
					qCounter--;
				else if (temp == '2' && qCounter != massiv.size() - 1)
					qCounter++;
				else if (temp == '0')
					break;
				else
				{
					cout << "\n\nNote:You are on last question\n";
					_getch();
				}
			}
		}
		catch (MyException ex)
		{
			cout << ex1.getError();
			_getch();
		}
		sourceFile.close();
	}
	void adminHome()
	{
		MyException ex1;
		Admin tempAdmin;
		string pass;
		Question que;
		char ch;
		int qCounter = 0;
		do
		{
			system("cls");
			cout << "\n\n\nAdministrator home";
			cout << "\n\nPress";
			cout << "\n1.Add question";
			cout << "\n2.View questions";
			cout << "\n3.Change password";
			cout << "\n4.Add user";
			cout << "\n5.Delete user";
			cout << "\n6.View users result";
			cout << "\n(0 to Logout)\n";
			ch = _getche();
			switch (ch)
			{
			case '1':
				que.newQuestion();
				_getch();
				break;
			case '2':
				tempAdmin.viewQuestion();
				break;
			case '3':
				system("cls");
				cout << "\n\nUsername::";
				cin >> tempAdmin.uName;
				encrypt(tempAdmin.uName);
				try
				{
					if (!getPassword(pass))
					{
						ex1.setError("Cannot create account");
						throw ex1;
					}
					strcpy_s(tempAdmin.password, pass.c_str());
					encrypt(tempAdmin.password);
					ofstream sourceFile("login.txt", ios::binary | ios::out | ios::trunc);
					sourceFile.write((char*)&tempAdmin, sizeof(tempAdmin));
					sourceFile.close();
					cout << "\n\n\nCongratulations! You have successfully changed your account";
					cout << "\nPress any key to login.";
					_getch();
				}
				catch (MyException ex)
				{
					cout << ex.getError();
					_getch();
				}
				_getch();
				break;
			case '4':
				userRegister();
				break;
			case '5':
				deleteUser();
				break;
			case '6':
				displayUserList();
				_getch();
				break;
			case '0':
				cout << "\n\n\nLogging out of system";
				_getch();
				break;
			default:
				cout << "\n\n\nERR:Please enter appropriate choice.";
				_getch();
			}
		} while (ch != '0');
	}
	void newAccount()
	{
		Admin newAdmin;
		MyException ex1;
		cout << "\n\n\nNo administrator account available.";
		cout << "\nPlease create a new account";
		cout << "\n\nUsername::";
		char temp[20];
		cin >> temp;
		encrypt(temp);

		string pass;
		try
		{
			if (!User::getPassword(pass))
			{
				ex1.setError("Cannot create account");
				throw ex1;
			}
			strcpy_s(newAdmin.uName, temp);
			strcpy_s(newAdmin.password, pass.c_str());
			encrypt(newAdmin.password);
			ofstream sourceFile("login.txt", ios::binary | ios::out | ios::trunc);
			sourceFile.write((char*)&newAdmin, sizeof(newAdmin));
			sourceFile.close();
			cout << "\n\n\nCongratulations! You have successfully created your account";
			cout << "\nPress any key to login.";
			_getch();
		}
		catch (MyException ex)
		{
			cout << ex.getError();
			_getch();
		}
	}
	void userRegister()
	{
		MyException ex1;
		cout << "\n\nName::";
		cin >> m_name;
		string destPath;
		char temp[20];
		auto lambda = [&temp, &destPath]()
		{
			strcpy_s(temp, "");
			destPath.clear();
			cout << "\nUsername::";
			cin >> temp;
			if (strlen(temp) < 5)
			{
				cout << "\nLength of password must be greater than or equal to 5";
				_getch();
				return false;
			}
			destPath = destPath + temp + ".txt";
			encrypt(temp);
			ifstream sourceFile(destPath, ios::in | ios::binary);
			if (sourceFile.is_open())
			{
				cout << "\nalready username";
				cout << "\nplease change username!!";
				_getch();
				return false;
			}
			return true;
		};
		while (!lambda()) {}
		setUserName(temp);
		setAge();
		string pass;
		try
		{
			if (!User::getPassword(pass))
				throw ex1;

			strcpy_s(m_userPass, pass.c_str());
			encrypt(m_userPass);
		}
		catch (MyException ex)
		{
			User::getPassword(pass);
		}

		ofstream destFile(destPath, ios::binary);
		destFile.write((char*)this, sizeof(*this));
		destFile.close();

		ofstream fin("users.txt", ios::binary | ios::app);
		fin << destPath << endl;
		fin.close();
	}
	void deleteUser()
	{
		displayUserList();

		string line, str;
		ofstream destFile2, outFile;
		ifstream destFile, inFile;
		cout << "\nEnter username::";
		cin >> str;
		str += ".txt";

		destFile.open("users.txt", ios::in | ios::binary);
		outFile.open("text.txt", ios::out | ios::binary);

		while (getline(destFile, line))
		{
			if (line != str)
				outFile << line << endl;
		}

		outFile.close();
		destFile.close();

		system("del users.txt");

		inFile.open("text.txt", ios::in | ios::binary);
		destFile2.open("users.txt", ios::out | ios::binary);
		while (getline(inFile, line))
			destFile2 << line << endl;

		destFile2.close();
		remove(str.c_str());
		system("del text.txt");
		displayUserList();
		_getch();
	}
	bool login()
	{
		Admin tempAdmin;
		string uTemp, pTemp;
		MyException ex1;
		system("cls");
		cout << "Login";
		try
		{
			ifstream adminFile("login.txt", ios::binary | ios::in);
			if (!adminFile.is_open())
			{
				ex1.setError("Cannot Open login.txt");
				throw ex1;
			}
			adminFile.read((char*)&tempAdmin, sizeof(tempAdmin));
			if (adminFile.good())
				adminFile.seekg(0, ios::beg);
			else
			{
				adminFile.close();
				newAccount();
				return false;
			}
			cout << "\n\n\nUser Name::";
			cin >> uTemp;
			if (uTemp != decryptkey(tempAdmin.uName))
			{
				ex1.setError("The username you've entered doesn't match any account.");
				throw ex1;
			}
			if (!getPassword(pTemp))
			{
				ex1.setError("Cannot login");
				throw ex1;
			}
			adminFile.read((char*)&tempAdmin, sizeof(tempAdmin));
			if (strcmp(pTemp.c_str(), decryptkey(tempAdmin.password)) == 0)
			{
				adminFile.close();
				return true;
			}
			else
			{
				cout << "\n\nPassword incorrect";
				_getch();
			}
			adminFile.close();
			return false;
		}
		catch (MyException ex)
		{
			cout << ex.getError();
			_getch();
			return false;
		}
	}
};
int main()
{
	srand(time(NULL));
	Admin aObj;
	User uObj;
	char choice;
	do
	{
		system("cls");
		showConsoleCursor(false);
		cout << "\n\nPress\n1 for Admin\n2 for User\n(0 to exit)\n";
		choice = _getche();
		switch (choice)
		{
		case '1':
			if (aObj.login())
				aObj.adminHome();
			break;
		case '2':
			uObj.userHome();
			break;
		case '0':
			exit(0);
		default:
			cout << "\n\n\nERR:Please enter appropriate choice.";
			_getch();
			continue;
		}
	} while (choice != 0);

	return 0;
}