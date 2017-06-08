import { createAction } from 'redux-actions';

export const UI = {
  TOGGLE_SIDEBAR: 'ui/TOGGLE_SIDEBAR',
  SET_SIDEBAR_VISIBILITY: 'ui/SET_SIDEBAR_VISIBILITY',
  SELECT_VIEW: 'ui/SELECT_VIEW'
}

export const toggleSidebar = createAction(UI.TOGGLE_SIDEBAR); // No payload
export const setSidebarVisibility = createAction(UI.SET_SIDEBAR_VISIBILITY); // isOpen
export const selectView = createAction(UI.SELECT_VIEW); // view
