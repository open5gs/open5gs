import { createAction } from 'redux-actions';

export const UI = {
  TOGGLE_SIDEBAR: 'ui/TOGGLE_SIDEBAR',
  SET_SIDEBAR_VISIBILITY: 'ui/SET_SIDEBAR_VISIBILITY',
  SELECT_VIEW: 'ui/SELECT_VIEW'
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
