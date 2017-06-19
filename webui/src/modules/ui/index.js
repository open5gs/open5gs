import { createAction } from 'redux-actions';
import { handleActions } from 'redux-actions';

export const UI = {
  TOGGLE_SIDEBAR: 'ui/TOGGLE_SIDEBAR',
  SET_SIDEBAR_VISIBILITY: 'ui/SET_SIDEBAR_VISIBILITY',
  SELECT_VIEW: 'ui/SELECT_VIEW',
  TEST_UI: 'ui/SELECT_VIEW'
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
export const toggleSidebar = createAction(UI.TOGGLE_SIDEBAR);
export const setSidebarVisibility = createAction(UI.SET_SIDEBAR_VISIBILITY);
export const selectView = createAction(UI.SELECT_VIEW);
export const testUI = ({type:UI.TEST_UI, argu:true})

const initialState = {
  sidebar: {
    isOpen: false,
    view: "subscriber"
  }
}

export default handleActions({
  [UI.TOGGLE_SIDEBAR]: (state, action) => ({ 
    ...state,
    sidebar: {
      ...state.sidebar,
      isOpen: !state.sidebar.isOpen
    }
  }),
  [UI.SET_SIDEBAR_VISIBILITY]: (state, action) => ({
    ...state,
    sidebar: {
      ...state.sidebar,
      isOpen: action.payload
    }
  }),
  [UI.SELECT_VIEW]: (state, action) => ({
    ...state,
    sidebar: {
      ...state.sidebar,
      view: action.payload
    }
  })
}, initialState);
