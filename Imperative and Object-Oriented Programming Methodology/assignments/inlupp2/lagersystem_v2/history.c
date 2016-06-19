#include <stdio.h>

#include "history.h"
#include "queue.h"


////////////////////////// Structs & Datatypes //////////////////////////

/**
 *  \struct history_state_s 
 *
 *  History state. Bundles relevant data with associated
 *  undo/redo- and `free` function for easy (and general)
 *  historical undo/redo.
 *
 *  \param data      Pointer to undo/redo relevant data
 *  \param freeData  Function pointer to function for freeing data pointer
 *  \param handler   Function pointer to send data and action type to
 */
struct history_state_s
{
  void* data;
  history_freeData freeData;
  history_handler handler;
};

/**
 *  \struct history_s 
 *
 *  History object. 
 *
 *  ### Default amount of saved steps are:
 *
 *  **20 steps undo**
 *
 *  **20 steps redo**
 *
 *  These may be changed using `history_setUndoLimit` and `history_setRedoLimit`.
 *
 *  \param undo  Stack-type queue containing undo-states/steps
 *  \param redo  Stack-type queue containing redo-states/steps
 */
struct history_s
{
  unsigned int undoLimit;
  unsigned int redoLimit;
  queue_t* undo;
  queue_t* redo;
};

/////////////////////// Hidden (internal) functions ///////////////////

/**
 *  \brief Clears a queue
 *
 *  Clears a queue from elements, calling
 *   each state's freeData-function.
 *
 *  \param queue Queue to clear
 *  \return void
 */
void _history_clearQueue(queue_t* queue)
{
  if(queue == NULL)
    {
      return;
    }

  while(queue_top(queue))
    {
      history_state_t* state = (history_state_t*) queue_pop(queue);
      history_freeState(state);
    }
}

/**
 *  \brief Verifies history object
 *
 *  Verifies that a history object is
 *  not empty.
 *
 *  \param history History object to verify
 *  \return bool True if OK, False otherwise
 */
bool _history_verifyHistory(history_t* history)
{
  return (history       != NULL &&
          history->undo != NULL &&
          history->redo != NULL);
}

/**
 *  \brief Verifies state object
 *
 *  Verifies that a history state object
 *  has been populated.
 *
 *  \param state State to verify
 *  \return bool True if OK, False otherwise
 */
bool _history_verifyState(history_state_t* state)
{
  return (state           != NULL &&
          state->data     != NULL && 
          state->handler  != NULL &&
          state->freeData != NULL);
}

/**
 *  \brief Handles action performed on state
 *
 *  Handles action performed on state since this
 *  action is near identical for both **undo** 
 *  and **redo**.
 *
 *  State popped from the `from` queue (if existing)
 *  will be pushed onto the `to` queue as to maintain
 *  the *undo/redo* relationship.
 *
 *  \param from        Queue to pop state from
 *  \param to          Queue to push popped state from
 *  \param actionFlag  Action flag to dend to handler function
 *  \return bool       True if action was possible, False otherwise
 */
bool _history_handleState(queue_t* from, queue_t* to, int actionFlag)
{
  if(queue_top(from)) {
    history_state_t* state = (history_state_t*) queue_pop(from);
    state->handler(state->data, actionFlag);
    queue_push(to, state);
    return true;
  }
  return false;
}

/**
 *  \brief Adjusts queue length according to limit
 *
 *  Adjusts the queue length according to limit by
 *  popping the last element in the queue and calling
 *  `history_freeState`.
 *
 *  \param queue Queue to adjust
 *  \param limit Limit of queue (how many steps to save)
 *  \return 
 */
void _history_adjustQueue(queue_t* queue, unsigned int limit) {
  while(queue_length(queue) > limit) {
    history_state_t* s = (history_state_t*) queue_popLast(queue);
    history_freeState(s);
  }
}

/////////////////////// Function implementations ///////////////////////////

history_t* history_new()
{
  history_t* new = malloc(sizeof(history_t));
  if(new != NULL)
    {
      new->undoLimit = 20;
      new->redoLimit = 20;
      new->undo = queue_newFILO();
      new->redo = queue_newFILO();
      if(new->undo == NULL || new->redo == NULL) {
        history_free(new);
        new = NULL;
      }
    }
  return new;
}

void history_free(history_t* history)
{
  if(history != NULL)
    {
      _history_clearQueue(history->undo);
      queue_destroy(history->undo);
      _history_clearQueue(history->redo);
      queue_destroy(history->redo);
      free(history);
    }
}

void history_setUndoLimit(history_t* history, unsigned int limit)
{
  if(history != NULL)
    {
      history->undoLimit = limit;
    }
}

void history_setRedoLimit(history_t* history, unsigned int limit)
{
  if(history != NULL)
    {
      history->redoLimit = limit;
    }
}

void history_add(history_t* history, history_state_t* state)
{
  if(_history_verifyHistory(history) && _history_verifyState(state))
    {
      queue_push(history->undo, state);
      _history_adjustQueue(history->undo, history->undoLimit);
      _history_clearQueue(history->redo);
      return;
    }
  
  fprintf(stderr, "Malformed history state in history_add");
}

void history_clear(history_t* history)
{
  if(_history_verifyHistory(history))
    {
      _history_clearQueue(history->undo);
      _history_clearQueue(history->redo);
    }
}

bool history_undo(history_t* history)
{
  if(_history_verifyHistory(history))
    {
      bool status = _history_handleState(history->undo, history->redo, HISTORY_ACTION_UNDO);
      _history_adjustQueue(history->redo, history->redoLimit);
      return status;
    }
  return false;
}

bool history_redo(history_t* history)
{
  if(_history_verifyHistory(history))
    {
      return _history_handleState(history->redo, history->undo, HISTORY_ACTION_REDO);
      _history_adjustQueue(history->undo, history->undoLimit);
    }
  return false;
}


//////////////////////////// State functions  ////////////////////////////

history_state_t* history_newState()
{
  history_state_t* new = malloc(sizeof(history_state_t));
  if(new != NULL)
    {
      new->data = NULL;
      new->handler = NULL;
      new->freeData = NULL;
    }
  return new;
}

void history_freeState(history_state_t* state)
{
  if(state != NULL)
    {
      if(state->data != NULL && state->freeData != NULL)
        {
          state->freeData(state->data);
        }
      free(state);
    }
}

bool history_setData(history_state_t* state, void* data)
{
  if(state != NULL && data != NULL)
    {
      state->data = data;
      return true;
    }
  return false;
}

bool history_setHandler(history_state_t* state, history_handler handler)
{
  if(state != NULL && handler != NULL)
    {
      state->handler = handler;
      return true;
    }
  return false;
}

bool history_setFreeFunction(history_state_t* state, history_freeData freeData)
{
  if(state != NULL && freeData != NULL)
    {
      state->freeData = freeData;
      return true;
    }
  return false;
}
