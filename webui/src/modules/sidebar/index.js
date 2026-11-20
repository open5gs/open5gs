import { createAction } from 'redux-actions';
import { handleActions } from 'redux-actions';

export const SIDEBAR = {
  TOGGLE: 'sidebar/TOGGLE',
  SET_VISIBILITY: 'sidebar/SET_VISIBILITY',
  SELECT_VIEW: 'sidebar/SELECT_VIEW',
  OPEN_POOL: 'sidebar/OPEN_POOL'
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

/*
  openPool: go to the CHF Shared Pools view with one pool open in its editor,
  from a SIM's quota on the CHF Quota page.  payload: the pool's id, or
  null once the CHF Shared Pools view has opened it.
*/
export const openPool = createAction(SIDEBAR.OPEN_POOL);

const initialState = {
  isOpen: false,
  view: "subscriber",
  openPool: null
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
  }),
  [SIDEBAR.OPEN_POOL]: (state, action) => ({
    ...state,
    view: action.payload ? "chfpool" : state.view,
    openPool: action.payload || null
  })
}, initialState);
