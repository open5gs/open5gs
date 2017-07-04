import { NOTIFICATION } from './actions';

const initialState = [];

function notifications(state = initialState, action) {
  switch(action.type) {
    case NOTIFICATION.SHOW:
      const { type, ...rest } = action;
      return [
        ...state,
        { ...rest, uid: action.uid }
      ];
    case NOTIFICATION.HIDE:
      return state.filter(notification => {
        return notification.uid !== action.uid;
      });
    case NOTIFICATION.CLEAR:
      return [];
  }
  return state;
}

export default notifications;
