#ifndef HISTORY
#define HISTORY

#include <stdbool.h>

/**
 *   \file history.h
 *   \brief Handles history/undo
 *  \author Karl Johansson
 *  \author Simon Lövgren
 *
 */

// Macros
#define HISTORY_ACTION_UNDO 1
#define HISTORY_ACTION_REDO 2

// Datatypes

/**
 *  \typedef
 *
 *  Funcion ponter to function for freeing
 *  data stored in state (used when clearing
 *  history amongst others).
 *
 *  Example function prototype:
 *     void myFreeFunction(void* data);
 *
 *  \note  standard `free` may be passed if
 *         no custom function is needed.
 *
 *  \param data void pointer to data
 *  \return void
 */
typedef void (*history_freeData)(void* data);

/**
 *  \typedef
 * 
 *  Function that will be called on undo and
 *  potential redo.
 *
 *  Example function prototype:
 *     void myHistoryHandler(void* data, int action);
 *
 *  \note Use history macros in function to
 *        take appropriate action:
 *        **HISTORY_ACTION_UNDO** when *undo* is called
 *        **HISTORY_ACTION_REDO** when *redo* is called
 *
 *  \param data void pointer to data
 *  \param action int denoting what action to take. 
 *  \return void
 */
typedef void (*history_handler)(void* data, int action);

/**
 *  \typedef
 *
 *  State that can be pushed to history.
 *  Note that for a state to be accepted by the
 *  history module, it must contain:
 *
 *  * Pointer to data (use `history_setData`)
 *  * Pointer to handler function (use `history_setHandler`)
 *  * Pointer to data `free`-function (use `history_setFreeFunction`)
 */
typedef struct history_state_s history_state_t;

/**
 *  \typedef
 *
 *  History object
 *
 *  Default limits saved steps are:
 *  ------------------------------
 *
 *  **20 steps undo**
 *
 *  **20 steps redo**
 */
typedef struct history_s history_t;


// Functions

/**
 *  \brief Create new history object
 *
 *  Creates a new history object and returns
 *  a pointer to said object.
 *
 *  \return history_t* History object
 */
history_t* history_new();

/**
 *  \brief Free history object
 *
 *  Free a history object from memory
 *
 *  \param history History to be freed
 *  \return void
 */
void history_free(history_t* history);

/**
 *  \brief Set maximum undo steps to save
 *
 *  \param history History to be limited
 *  \param limit Max stored steps
 *  \return void
 */
void history_setUndoLimit(history_t* history, unsigned int limit);


/**
 *  \brief Set maximum redo steps to save
 *
 *  \param history History to be limited
 *  \param limit Max stored steps
 *  \return void
 */
void history_setRedoLimit(history_t* history, unsigned int limit);

/**
 *  \brief Add history state
 *
 *  \warning  This action will clear all redoable actions
 *            of the supplied history object.
 *
 *  \param history History to add state to
 *  \param state Prepared history state (see typedef)
 *  \return void
 */
void history_add(history_t* history, history_state_t* state);

/**
 *  \brief Clear stored history
 *
 *  \param history History to clear
 *  \return void
 */
void history_clear(history_t* history);

/**
 *  \brief Undo last (stored) action.
 *
 *  Undo last stored action.
 *
 *  \param history History to use states from
 *  \return bool True if undo is possible, False otherwise
 */
bool history_undo(history_t* history);

/**
 *  \brief Redo
 *
 *  Redo last undone action.
 *
 *  \note  If some other action
 *         than redo is performed after last undo that
 *         alters the history state (eg. adds history state), 
 *         the redo-stack will be cleared.
 *
 *  \param history History to use states from
 *  \return bool True if redo is possible, False otherwise
 */
bool history_redo(history_t* history);

// State creation

/**
 *  \brief Creates a new state
 *
 *  Creates a new history state and returns
 *  a pointer to said state.
 *
 *  \return history_state* Pointer to new history state
 */
history_state_t* history_newState();

/**
 *  \brief Frees a state from memory
 *
 *  Frees a state from memory.
 *
 *  \Warning  If no function pointer is supplied
 *            for freeing data, the data must be
 *            freed 'manually' to circumvent
 *            memory leekage.
 *
 *  \param Pointer to history state
 *  \return void
 */
void history_freeState(history_state_t* state);

/**
 *  \brief Sets pointer to dataset
 *
 *  Sets pointer to dataset used by (in state) accompanied
 *  functions for undo/redo action and freeing of data.
 *
 *  This pointer is fed to the `handler` and `freeData`
 *  functions.
 *
 *  \param param
 *  \return return type
 */
bool history_setData(history_state_t* state, void* data);

/**
 *  \brief Set action handler function of state
 *
 *  \param state   State to add handler to
 *  \param handler Function pointer to a handler (history_handler)
 *  \return bool True if success, False otherwise
 */
bool history_setHandler(history_state_t* state, history_handler handler);

/**
 *  \brief Set `free`-function for data pointed to from state
 *
 *  Set `free`-function for data pointed to from state. 
 *
 *  \note If the data pointed to is non-complex
 *        the `free` function from `stdlib.h` may
 *        be passed in lue of a custom function.
 *
 *  \param state   State to add handler to
 *  \param handler Function pointer to a data `free`-function
 *  \return bool True if success, False otherwise
 */
bool history_setFreeFunction(history_state_t* state, history_freeData freeData);

#endif
