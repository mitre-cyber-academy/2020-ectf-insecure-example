#!/usr/bin/python3

"""
A simple test file to play with JSON data in Python
"""

import json     						# For working with JSON data
from random import randint              # For random integer generation

""" Testing lists and dicts in Python and JSON
# Make a last of fruits
fruits = ["apple", "orange", "banana"];

# Print list of fruits
print("Python List: ", fruits)

# Encode list of fruits as JSON
jsonFruits = json.dumps(fruits)

# Print JSON encoded list of fruits
print("JSON List: ", jsonFruits)

# Create a dictionary of fruit and color
fruitDict = {"apple":"red", "orange":"orange", "banana":"yellow"}

# Print dictionary of fruit and color
print("Python Dict: ", fruitDict)

# Encode list dictionary of fruit and color as JSON
jsonFruitDict = json.dumps(fruitDict)

# Print JSON encoded dictionary of fruit and color
print("JSON Dict: ", jsonFruitDict)
"""

# Create dictionary with empty lists for fruits and vegetables
fruitDict = {"fruits":[], "vegetables":[]}

# Create list of fruits
fruits = ["apple", "banana", "kiwi"]

# Create list of vegetables
vegetables = ["squash", "tomato", "potato"]

# Add fruits to dictionary with random quantity
for fruit in fruits:
	quantity = randint(10,100)
	fruitObject = {"name": fruit, "quantity": quantity}
	fruitDict["fruits"].append(fruitObject)

# Add fruits to dictionary with random quantity
for vegetable in vegetables:
	quantity = randint(10,100)
	vegetableObject = {"name": vegetable, "quantity": quantity}
	fruitDict["vegetables"].append(vegetableObject)


print(json.dumps(fruitDict, indent=2))


# This produces what we're looking for.