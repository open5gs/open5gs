import {RNS_SHOW_NOTIFICATION, RNS_HIDE_NOTIFICATION, RNS_REMOVE_ALL_NOTIFICATIONS} from './const';

export default function Notifications(state = [], action = {}) {
  switch(action.type) {
    case RNS_SHOW_NOTIFICATION:
      const { type, ...rest } = action;
      return [
        ...state,
        { ...rest, uid: action.uid}
      ];
    case RNS_HIDE_NOTIFICATION:
      return state.filter(notification => {
        return notification.uid !== action.uid;
      });
    case RNS_REMOVE_ALL_NOTIFICATIONS:
      return [];
  }
  return state;
}
