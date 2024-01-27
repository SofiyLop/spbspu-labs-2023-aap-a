#include "inputShapes.hpp"
#include "rectangle.hpp"
#include "ring.hpp"
#include "ellipse.hpp"
#include <iostream>
#include <string>

novokhatskiy::Shape **novokhatskiy::inputShapes(std::istream &input, size_t &shapeCounter)
{
  std::string shapesNames[3] = {"RECTANGLE", "RING", "ELLIPSE"};
  size_t rightShapesParametersCount[3] = {4, 4, 4};
  std::string currentName = "";
  double *currentParameters = nullptr;
  novokhatskiy::Shape **currentShapes = nullptr;
  novokhatskiy::Shape **oldShapes = nullptr;
  char symbol = 0;
  while (input >> currentName)
  {
    for (size_t i = 0; i < 3; ++i)
    {
      if (currentName == shapesNames[i])
      {
        try
        {
          currentParameters = new double[rightShapesParametersCount[i]];
        }
        catch (const std::bad_alloc &)
        {
          if (currentShapes != nullptr)
          {
            for (size_t i = 0; i < shapeCounter; i++)
            {
              delete currentShapes[i];
            }
            delete[] currentShapes;
          }
          throw;
        }
        for (size_t j = 0; j < rightShapesParametersCount[i]; j++)
        {
          input >> currentParameters[j];
        }

        if (!input)
        {
          if (currentShapes != nullptr)
          {
            for (size_t i = 0; i < shapeCounter; i++)
            {
              delete currentShapes[i];
            }
            delete[] currentShapes;
          }
          delete[] currentParameters;
          throw std::invalid_argument("Wrong arguments");
        }
        oldShapes = currentShapes;
        currentShapes = new Shape *[shapeCounter + 1];
        if (oldShapes)
        {
          for (size_t i = 0; i < shapeCounter; i++)
          {
            currentShapes[i] = oldShapes[i];
          }
        }
        delete[] oldShapes;
        try
        {
          if (currentName == "RECTANGLE")
          {
            currentShapes[shapeCounter] = new Rectangle({currentParameters[0], currentParameters[1]}, {currentParameters[2], currentParameters[3]});
          }
          else if (currentName == "RING")
          {
            currentShapes[shapeCounter] = new Ring({currentParameters[0], currentParameters[1]}, currentParameters[2], currentParameters[3]);
          }
          else if (currentName == "ELLIPSE")
          {
            currentShapes[shapeCounter] = new Ellipse({currentParameters[0], currentParameters[1]}, currentParameters[2], currentParameters[3]);
          }
          ++shapeCounter;
        }
        catch (const std::bad_alloc &)
        {
          delete[] currentParameters;
          for (size_t i = 0; i < shapeCounter; i++)
          {
            delete currentShapes[i];
          }
          delete[] currentShapes;
          throw;
        }
        catch (const std::exception &e)
        {
          std::cerr << e.what() << '\n';
        }
        delete[] currentParameters;
      }
    }
    if (currentName == "")
    {
      std::cerr << "Wrong input";
    }
    if (currentName == "SCALE")
    {
      break;
    }
    input >> std::noskipws;
    while (symbol != '\n')
    {
      input >> symbol;
    }
    input >> std::skipws;
  }
  return currentShapes;
}
