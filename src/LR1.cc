/*
    This file is part of Ellerre

    Ellerre is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ellerre is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Ellerre. If not, see <http://www.gnu.org/licenses/>.
*/
#include "LR1.h"

LR1::LR1()
{
  this->type = "LR1"; 
}

LR1::LR1(Grammar* grammar)
{
  this->type = "LR1";
  this->grammar = grammar;
  // expand grammar
  this->grammar->expand_grammar();

  // calculate first and follow sets
  this->follow = this->grammar->follow();
  this->first = this->grammar->first();
  std::cout << "first: \n";
  grammar->print_first_sets();
  std::cout << "follow: \n";
  grammar->print_follow_sets();
  // create item set from input grammar
  create_item_set();
  create_automata();
}

LR1::~LR1()
{
}

void LR1::create_item_set(void) 
{
  // Produce the LR0 item set
  std::vector<Rule*> r = this->grammar->rules; 
  std::vector<Rule*>::iterator it_r;
  std::vector<Symbol*> body;
  std::vector<Symbol*>::iterator it_b;
  int dot;
  bool hasNext;

  // for each rule in grammar
  for (it_r = r.begin(); it_r != r.end(); it_r++) {
    hasNext = true;
    body = (*it_r)->body;
    dot = 0;

    // if the rule produces the empty symbol
    if(body.size() == 1 && body[0]->empty) {
      hasNext = false;
      Rule *r = new Rule((*it_r)->head);
      Item *i = new Item(r, std::make_tuple(0, hasNext, body[0]));
      addItem(i);
    } else {
      // for each symbol in rule add a dot
      for (it_b = body.begin(); it_b != body.end(); it_b++) {
        Item *i = new Item((*it_r), std::make_tuple(dot, hasNext, (*it_b)));
        addItem(i);
        dot++;
      }

      // the last dot does not precede any symbol
      hasNext = false;
      it_b--;
      Item *i = new Item((*it_r), std::make_tuple(dot, hasNext, (*it_b)));
      addItem(i);
    }
  }

  // Get the follow set to determine LR1 lookaheads in the items
  std::map<Symbol*,std::set<Symbol*> > follows = this->grammar->follow();
  std::set<Item*> items_lookahead;

  // For each item in the LR(0) item set, apply each element of the follow set as a lookahead
  for(Item *i:  this->items){
    // for each entry <Head,follow> in the follow set
    for (auto& entry : follows) {
      // add the follow lookahead symbols for the head symbol of our items that matches entry.first 
      if(*(entry.first) == *i->rule->head) {
        // for each follow symbol for a given non terminal
        for (Symbol *s : entry.second) {
          std::vector<Symbol*> l{s};
          items_lookahead.insert(new Item(i->rule, i->dot, l)); 
        }        
      }
    }
  }
  // remove previous LR0 item set and assign the item set with the lookaheads to this->items
  this->clearItems();
  setItems(items_lookahead);
}

void LR1::create_automata(void)
{
  // This assumes that the first item in closure is the Starting item rule from the augmented grammar located at this->items.begin()
  std::vector<State*> states;
  bool change = true;
  int last_size = 0;
  Item* start = *this->items.begin(); 
  std::set<Item*> kernel = {start}; 
  std::set<Item*> items_set = closure(kernel);

  // Create the first state with its id, kernel and item_set
  State* starting_state = new State(0, kernel, items_set);
  createTransitionStates(starting_state);
  this->states.insert(starting_state);
}

std::set<Item*> LR1::closure(std::set<Item*> kernel)
{

}

State* LR1::getTransitionState(State state, Symbol* s)
{

}


void LR1::createTransitionStates(State* state)
{

}

std::set<Item*> LR1::getProductionOfItem(Item* item)
{
    
}