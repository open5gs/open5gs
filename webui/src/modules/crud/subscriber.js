import {
  fetchCollection
} from './actions'

const MODEL = 'subscribers';
const ID = 'imsi';
const URL = '/Subscriber';

export const fetchSubscribers = (params = {}) => {
  return fetchCollection(MODEL, ID, URL, params)
}
