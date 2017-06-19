import {
  fetchCollection
} from 'actions/crud'

const MODEL = 'subscribers';
const ID = 'imsi';
const URL = '/Subscriber';

export const fetchSubscribers = (params = {}) => {
  return fetchCollection(MODEL, ID, URL, params)
}