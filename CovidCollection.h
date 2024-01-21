/*
  Name: Temitope Oladimeji Samuel Adebayo
  Date: 2023-11-09

  I have done all the coding by myself and only copied the code that my professor provided 
  to complete my workshops and assignments.
*/

#ifndef SDDS_COVIDCOLLECTION_H
#define SDDS_COVIDCOLLECTION_H

#include <iostream>
#include <vector>
#include <list>

namespace sdds {
  const int epidemicStatus = 300;
  const int earlyStatus = 50;
  // a structure that holds the information about a covid case
  struct Covid
  {
    std::string m_country{};
    std::string m_city{};
    std::string m_variant{};
    int m_year{};
    size_t m_cases{};
    size_t m_deaths{};
    std::string m_severity = "General";
  };
  
  // a class that holds a collection of covid cases
  class CovidCollection {
      std::vector<Covid> m_collection{};
    public:
      // a one-argument constructor that receives the name of the file to load
      CovidCollection(const std::string filename);
      // a query that prints the collection to the output stream without manual loops, 
      // accepts a reference to an output stream and the name of the country to display
      void display(std::ostream& out, const std::string& country = "ALL") const;
      // a function that removes leading and trailing spaces from a string
      void trim(std::string& str);
      // sort the collection of Covid objects in ascending order based on the field received as parameter
      void sort(const std::string& field = "country");
      // searches the collection for a city in the specified country where the variant from the first parameter has caused more deaths than the last parameter.
      bool inCollection(const std::string& variant, const std::string& country, unsigned int deaths) const;
      // create and return a collection of Covid objects from the current instance where the number of deaths is at least as the value specified as parameter
      std::list<Covid> getListForDeaths(unsigned int deaths) const;
      // updates the status of each city
      void updateStatus();
  };

  // a helper operator that prints the covid case to the output stream
  std::ostream& operator<<(std::ostream& out, const Covid& theCovid);
}

#endif