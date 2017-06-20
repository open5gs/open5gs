import {
  fetchCollection
} from './actions'

const MODEL = 'subscribers';
const URL = '/Subscriber';

export const fetchSubscribers = (params = {}) => {
  return fetchCollection(MODEL, URL, params, { idProperty: 'imsi' });
}
