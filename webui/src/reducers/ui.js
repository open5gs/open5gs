import { handleActions } from 'redux-actions';
import { UI } from 'actions/ui';

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