import { createAction, handleActions } from 'redux-actions';
import { Map } from 'immutable';

const TOGGLE = 'sidebar/TOGGLE';
const SET_VISIBILITY = 'sidebar/SET_VISIBILITY';
const SET_VIEW = 'sidebar/SET_VIEW';

export const toggle = createAction(TOGGLE); // No payload
export const setVisibility = createAction(SET_VISIBILITY); // visible
export const setView = createAction(SET_VIEW); // view

const initialState = Map({
  visible: false,
  view: 'pdn'
});

export default handleActions({
  [TOGGLE]: (state, action) => state.set('visible', !state.get('visible')),
  [SET_VISIBILITY]: (state, action) => state.set('visible', action.payload),
  [SET_VIEW]: (state, action) => state.set('view', action.payload)
}, initialState);