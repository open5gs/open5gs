import { createAction, handleActions } from 'redux-actions';
import { Map } from 'immutable';

const TOGGLE = 'sidebar/TOGGLE';
const SET_VISIBILITY = 'sidebar/SET_VISIBILITY';
const SET_VIEW = 'sidebar/SET_VIEW';

export const toggle = createAction(TOGGLE); // No payload
export const setVisibility = createAction(SET_VISIBILITY); // isOpen
export const setView = createAction(SET_VIEW); // view

const initialState = Map({
  isOpen: false,
  view: 'pdn'
});

export default handleActions({
  [TOGGLE]: (state, action) => state.set('isOpen', !state.get('isOpen')),
  [SET_VISIBILITY]: (state, action) => state.set('isOpen', action.payload),
  [SET_VIEW]: (state, action) => state.set('view', action.payload)
}, initialState);