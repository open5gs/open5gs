import { createStore } from 'redux';

import reducers from './modules';

export const initStore = () => {
  return createStore(reducers);
}