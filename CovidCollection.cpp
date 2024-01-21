/*
  Name: Temitope Oladimeji Samuel Adebayo
  Date: 2023-11-09

  I have done all the coding by myself and only copied the code that my professor provided 
  to complete my workshops and assignments.
*/

#include "CovidCollection.h"
#include <fstream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iomanip>
#include <string>

using namespace std;

namespace sdds {
  CovidCollection::CovidCollection(const std::string filename){
    ifstream file(filename);
    if (file.is_open()) {
      Covid temp;
      std::string line;

      while(getline(file, line)) {
        // extract the country name
        temp.m_country = line.substr(0, 25);
        trim(temp.m_country);

        // extract the city name
        temp.m_city = line.substr(25, 25);
        trim(temp.m_city);

        // extract the variant name
        temp.m_variant = line.substr(50, 25);
        trim(temp.m_variant);

        // extract the year
        temp.m_year = stoi(line.substr(75, 5));

        // extract the case number
        temp.m_cases = stoul(line.substr(80, 5));

        // extract the death number
        temp.m_deaths = stoul(line.substr(85, 5));

        // add the covid case to the collection
        m_collection.push_back(temp);
      }

      file.close();
    } else {
      throw "Cannot open file " + filename;
    }
  }

  void CovidCollection::trim(std::string& str) {
    // remove leading spaces
    while (str[0] == ' ') {
      str.erase(0, 1);
    }

    // remove trailing spaces
    while (str[str.length() - 1] == ' ') {
      str.erase(str.length() - 1, 1);
    }
  }

  void CovidCollection::display(std::ostream& out, const std::string& country) const {
    size_t totalCases = 0;
    size_t totalDeaths = 0;
    size_t countryCases = 0;
    size_t countryDeaths = 0;

    // for_each and accumulate

    // check if the country is ALL, if it is, display all the covid cases, otherwise, display only the covid cases for the country
    if (country == "ALL") {
      // loop through the collection and add the cases and deaths to the totalCases and totalDeaths variables
      for_each(m_collection.begin(), m_collection.end(), [&](const Covid& covid) {
        out << covid << endl;
        totalCases += covid.m_cases;
        totalDeaths += covid.m_deaths;
      });

      std::string totalCasesStr = "Total cases around the world: " + std::to_string(totalCases);
      std::string totalDeathsStr = "Total deaths around the world: " + std::to_string(totalDeaths);

      out << "| " << setw(84) << right << totalCasesStr << " |" << endl;
      out << "| " << setw(84) << right << totalDeathsStr << " |" << endl;
    } else {
			out << "Displaying information of country = " << country << endl;

      // loop through the collection and add the cases and deaths to the totalCases and totalDeaths variables
      // also, display the covid cases for the country
      for_each(m_collection.begin(), m_collection.end(), [&](const Covid& covid) {
        totalCases += covid.m_cases;
        totalDeaths += covid.m_deaths;
        if (covid.m_country == country) {
          out << covid << endl;
          countryCases += covid.m_cases;
          countryDeaths += covid.m_deaths;
        }
      });

      std::string totalCasesStr = "Total cases in " + country + ": " + std::to_string(countryCases);
			std::string totalDeathsStr = "Total deaths in " + country + ": " + std::to_string(countryDeaths);

      // calculate the percentage of cases and deaths for the country by casting
			std::string countryPercentageStr = country + " has " + std::to_string(static_cast<double>(countryCases) / totalCases * 100) + "% of global cases and " + std::to_string(static_cast<double>(countryDeaths) / totalDeaths * 100) + "% of global deaths";
      
      out << setw(88) << setfill('-') << "" << endl;
      out << setfill(' ');
      out << "| " << setw(84) << right << totalCasesStr << " |" << endl;
      out << "| " << setw(84) << right << totalDeathsStr << " |" << endl;
      out << "| " << setw(84) << right << countryPercentageStr << " |" << endl;
    }
  }

  void CovidCollection::sort(const std::string& field) {
    // sort the collection based on the field received as a parameter
    std::sort(m_collection.begin(), m_collection.end(), [&](const Covid& one, const Covid& two) {
        if (field == "country") {
            if (one.m_country != two.m_country) {
                return one.m_country < two.m_country;
            } else {
                return one.m_deaths < two.m_deaths;
            }
        } else if (field == "city") {
            if (one.m_city != two.m_city) {
                return one.m_city < two.m_city;
            } else {
                return one.m_deaths < two.m_deaths;
            }
        } else if (field == "variant") {
            if (one.m_variant != two.m_variant) {
                return one.m_variant < two.m_variant;
            } else {
                return one.m_deaths < two.m_deaths;
            }
        } else if (field == "year") {
            if (one.m_year != two.m_year) {
                return one.m_year < two.m_year;
            } else {
                return one.m_deaths < two.m_deaths;
            }
        } else if (field == "cases") {
            if (one.m_cases != two.m_cases) {
                return one.m_cases < two.m_cases;
            } else {
                return one.m_deaths < two.m_deaths;
            }
        } else {
            // Default case, e.g., if field is not recognized
            return one.m_deaths < two.m_deaths;
        }
    });
  }

  bool CovidCollection::inCollection(const std::string& variant, const std::string& country, unsigned int deaths) const {
    // Use std::find_if with a lambda function to check if the condition is met
    auto res = std::find_if(m_collection.begin(), m_collection.end(), [&](const Covid& covid) {
      return (covid.m_variant == variant && covid.m_country == country && covid.m_deaths >= deaths);
    });

    // Check if the element was found
    return (res != m_collection.end());
  }

  std::list<Covid> CovidCollection::getListForDeaths(unsigned int deaths) const {
    std::list<Covid> list;

    copy_if(m_collection.begin(), m_collection.end(), back_inserter(list), [&](const Covid& covid) {
      return covid.m_deaths >= deaths;
    });

    return list;
  }

  void CovidCollection::updateStatus() {
    // update the status of each covid case
    std::transform(m_collection.begin(), m_collection.end(), m_collection.begin(), [&](Covid& covid) {
      if (covid.m_deaths > epidemicStatus) {
          covid.m_severity = "EPIDEMIC";
      } else if (covid.m_deaths < earlyStatus) {
          covid.m_severity = "EARLY";
      } else {
          covid.m_severity = "MILD";
      }
      return covid;
    });
  }

  std::ostream& operator<<(std::ostream& out, const Covid& theCovid) {
    out << "| " << setw(21) << left << theCovid.m_country << " | ";
    out << setw(15) << left << theCovid.m_city << " | ";
    out << setw(20) << left << theCovid.m_variant << " | ";
    if (theCovid.m_year > 0) {
      out << setw(6) << right << theCovid.m_year;
    } 
    else {
      out << setw(6) << right << setfill(' ') << " ";
    }
    out << " | " << setw(4) << right << theCovid.m_cases << " | ";
    out << setw(3) << right << theCovid.m_deaths << " | ";
    out << "| " << setw(8) << theCovid.m_severity << " |";

    return out;
  }
}