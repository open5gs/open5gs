import { createAction } from 'redux-actions';
import { handleActions } from 'redux-actions';

export const SIDEBAR = {
  TOGGLE: 'sidebar/TOGGLE',
  SET_VISIBILITY: 'sidebar/SET_VISIBILITY',
  SELECT_VIEW: 'sidebar/SELECT_VIEW'
}

/*
  UIAction.toggleSidebar
    payload : null

  UIAction.setSidebarVisibiliy
    payload : {
      isOpen
    } 

  UIAction.selectView
    payload : {
      view
    }
*/
export const toggle = createAction(SIDEBAR.TOGGLE);
export const setVisibility = createAction(SIDEBAR.SET_VISIBILITY);
export const selectView = createAction(SIDEBAR.SELECT_VIEW);

const initialState = {
  isOpen: false,
  view: "subscriber"
}

export default handleActions({
  [SIDEBAR.TOGGLE]: (state, action) => ({ 
    ...state,
    isOpen: !state.isOpen
  }),
  [SIDEBAR.SET_VISIBILITY]: (state, action) => ({
    ...state,
    isOpen: action.payload
  }),
  [SIDEBAR.SELECT_VIEW]: (state, action) => ({
    ...state,
    view: action.payload
  })
}, initialState);
