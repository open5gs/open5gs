import {
  fetchCollection
} from './actions'

const MODEL = 'subscribers';
const URL = '/Subscriber';

export const fetchSubscribers = (params = {}) => {
  return fetchCollection(MODEL, URL, params);
}

export const fetchSubscriber = (id, params = {}) => {
  return fetchDocument(MODEL, id, `${URL}/${id}`, params, { idProperty: 'imsi' });
}
