import {
  fetchCollection,
  fetchDocument
} from './actions'

const MODEL = 'subscribers';
const URL = '/Subscriber';

export const fetchSubscribers = (params = {}) => {
  return fetchCollection(MODEL, URL, params, { idProperty: 'imsi' });
}

export const fetchSubscriber = (imsi, params = {}) => {
  return fetchDocument(MODEL, imsi, `${URL}/${imsi}`, params, { idProperty: 'imsi' });
}

export const deleteSubscriber = (imsi, params = {}) => {
  return deleteDocument(MODEL, imsi, `${URL}/${imsi}`, params, { idProperty: 'imsi' });
}
