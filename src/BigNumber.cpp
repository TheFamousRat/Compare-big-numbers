
#include "BigNumber.hpp"

BigNumber::BigNumber()
{
    //Not much, we initialize both of the numbers to 0
    decimalPart.push_back(0);
    intPart.push_back(0);
}

BigNumber::BigNumber(std::string const& numberToTake)
{
    for (auto it = numberToTake.begin() ; it != numberToTake.end() ; it++)
    {
        if (*it != '0' && *it != '1' && *it != '2' && *it != '3' &&
           *it != '4' && *it != '5' && *it != '6' && *it != '7' &&
           *it != '8' && *it != '9')
        {
            std::cout << "Error from constructor : The number contains non-number characters !\n";
            std::cout << "Faulty character found : " << *it << '\n';
            throw "Error from constructor : The number contains non-number characters !\n";
        }
    }

}

BigNumber::BigNumber(std::string&& numberToTake)
{
    lastDecimalPartPower = 0;

    //We first check if there is any
    if (!checkStringIntegrity(numberToTake))
    {
        std::cout << "Error from constructor : The number contains non-number characters !\n";
        throw "Non-numbers characters\n";
    }

    switch (std::count(numberToTake.begin(), numberToTake.end(), '.'))
    {
    case 0: //Only an integer !
        decimalPart.push_back(0);//Not needed, so we put it to 0

        try{
            changeIntPart(numberToTake);
        }catch (std::string const& ex){
            std::cout << "Error in constructor : " << ex << "\nGenerating an empty intPart\n";
            changeIntPart("0");
        }

        break;
    case 1: //Both
        std::string decimalString;
        std::string intString;

        std::size_t dotPosition = numberToTake.find(".");
        intString = numberToTake.substr(0, dotPosition);
        decimalString = numberToTake.substr(dotPosition + 1);

        try{
            changeIntPart(intString);
        }catch (std::string const& ex){
            std::cout << "Error in constructor : " << ex << "\nGenerating an empty intPart\n";
            changeIntPart("0");
        }

        try{
            changeDecimalPart(decimalString);
        }catch (std::string const& ex){
            std::cout << "Error in constructor : " << ex << "\nGenerating an empty decimalPart\n";
            changeDecimalPart("0");
        }

        break;
    default:
        std::cout << "Error from constructor : Multiple decimal points found (found "
                  << std::count(numberToTake.begin(), numberToTake.end(), '.') << ")\n";
        throw "Multiple decimal points\n";
        break;
    }
}

void BigNumber::changeIntPart(std::string const& newIntPart)
{
    if (!checkStringIntegrity(newIntPart))
    {
        std::cout << "Error from changeIntPart : The number contains non-number characters !\n";
        throw "Non-numbers characters\n";
    }

    intPart.clear();

    std::string stringTreated;

    std::size_t last0Pos(0);

    //We must eliminate any zeros before the first digit
    for ( ; last0Pos < newIntPart.size() ; last0Pos++)
    {
        if (newIntPart[last0Pos] != '0')
            break;
    }

    stringTreated = newIntPart.substr(last0Pos);

    if (stringTreated.size() != 0)
    {
        std::stringstream takeChunks;

        const int rest = stringTreated.size() % MAX_DIGIT_NUMBER;
        const int numberOfIterations = (stringTreated.size() - (rest)) / MAX_DIGIT_NUMBER;
        int pos = (numberOfIterations - 1) * MAX_DIGIT_NUMBER + (rest);

        for ( ; rest <= pos ; pos-=MAX_DIGIT_NUMBER)
        {
            for (int j(0) ; j < MAX_DIGIT_NUMBER ; j++)
            {
                takeChunks << stringTreated[j + pos];
            }
            intPart.push_back(atoi(takeChunks.str().c_str()));
            takeChunks.str(std::string());
        }

        if (rest)
        {
            //Now we only have to add the rest
            for (int i(0) ; i < rest ; i++)
                takeChunks << stringTreated[i];

            intPart.push_back(atoi(takeChunks.str().c_str()));
            takeChunks.str(std::string());
        }
    }
    else
        intPart.push_back(0);
}

void BigNumber::changeDecimalPart(std::string const& newDecimalPart)
{

    if (!checkStringIntegrity(newDecimalPart))
    {
        std::cout << "Error from changeDecimalPart : The number contains non-number characters !\n";
        throw "Non-numbers characters\n";
    }

    decimalPart.clear();

    //Now we must remove any superficial 0 at the end of the string
    std::size_t last0Pos(0);

    for (last0Pos = newDecimalPart.size() - 1 ; last0Pos >= 0 ; last0Pos--)
    {
        if (newDecimalPart[last0Pos] != '0')
            break;
    }

    std::string stringTreated = newDecimalPart.substr(0, last0Pos + 1);

    if (stringTreated.size() != 0)
    {
        std::stringstream takeChunks;

        const int rest = stringTreated.size() % MAX_DIGIT_NUMBER;
        const int numberOfIterations = (stringTreated.size() - (rest)) / MAX_DIGIT_NUMBER;

        for (int i(0) ; i < numberOfIterations ; i++)
        {
            for (int j(0) ; j < MAX_DIGIT_NUMBER ; j++)
            {
                takeChunks << stringTreated[i * MAX_DIGIT_NUMBER + j];
            }

            decimalPart.push_back(atoi(takeChunks.str().c_str()));
            takeChunks.str(std::string());
        }

        if (rest)
        {
            //And then the rest
            takeChunks.str(std::string());
            for (int i(0) ; i <= rest ; i++)
            {
                takeChunks << stringTreated[stringTreated.size() - rest + i];
            }
            decimalPart.push_back(atoi(takeChunks.str().c_str()));

            lastDecimalPartPower = 0;
            while (takeChunks.str()[lastDecimalPartPower] == '0')
                lastDecimalPartPower++;


        }
        else//For the sake of the last digit's number of zeros
        {
            std::string lastString = stringTreated.substr(stringTreated.size() - MAX_DIGIT_NUMBER, MAX_DIGIT_NUMBER);

            lastDecimalPartPower = 0;
            while (lastString[lastDecimalPartPower] == '0')
                lastDecimalPartPower++;
        }
    }
    else
    {
        decimalPart.push_back(0);
        lastDecimalPartPower = 0;
    }
}

bool BigNumber::checkStringIntegrity(std::string const& toTest)
{
    for (auto it = toTest.begin() ; it != toTest.end() ; it++)
    {
        if (*it != '0' && *it != '1' && *it != '2' && *it != '3' &&
           *it != '4' && *it != '5' && *it != '6' && *it != '7' &&
           *it != '8' && *it != '9' && *it != '.')
        {
            return false;
        }
    }

    return true;
}

BigNumber::~BigNumber()
{

}

std::ostream& operator<<(std::ostream& os, const BigNumber& toStream)
{
    os << *toStream.intPart.rbegin();
    //We print the first integer. We do that because it is the only one who cannot have any zeros in front of him, so
    //there is no need for some useless testing

    for (auto it = toStream.intPart.rbegin() + 1 ; it != toStream.intPart.rend() ; it++)//It is necessary to print the integer part backwards
    {
        //Now, what we'll need to do is determine how many zeros we will need to put before the integer part
        if (*it < 10)
        {
            os << "000" << *it;
        }
        else if (*it < 100)
        {
            os << "00" << *it;
        }
        else if (*it < 1000)
        {
            os << "0" << *it;
        }
        else
        {
            os << *it;
        }
    }

    os << '.';

    for (auto it = toStream.decimalPart.begin() ; it != toStream.decimalPart.end() ; it++)
    {
        if (it != toStream.decimalPart.end() - 1)
        {
            if (*it < 10)
            {
                os << "000" << *it;
            }
            else if (*it < 100)
            {
                os << "00" << *it;
            }
            else if (*it < 1000)
            {
                os << "0" << *it;
            }
            else
            {
                os << *it;
            }
        }
        else
        {
            for (int i(0); i < toStream.lastDecimalPartPower ; i++)
                os << '0';


            os << *it;
        }
    }

    return os;
}

bool BigNumber::writeToFile(std::string filePath)
{
    std::ofstream writeFile;
    writeFile.open(filePath.c_str());

    if (writeFile.is_open())
    {
        writeFile << *this;
    }
    else
    {
        std::cout << "Error occurred during the file's opening (file name was (" << filePath << "). Failed to write the number\n";
        return false;
    }

    return true;
}

bool BigNumber::readFromFile(std::string filePath)
{

}