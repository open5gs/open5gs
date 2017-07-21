import {
  fetchCollection,
  fetchDocument,
  createDocument,
  updateDocument,
  deleteDocument
} from './actions'

export const MODEL = 'subscribers';
export const URL = '/Subscriber';

export const fetchSubscribers = (params = {}) => {
  return fetchCollection(MODEL, URL, params, { idProperty: 'imsi' });
}

export const fetchSubscriber = (imsi, params = {}) => {
  return fetchDocument(MODEL, imsi, `${URL}/${imsi}`, params, { idProperty: 'imsi' });
}

export const createSubscriber = (params = {}, data = {}) => {
  return createDocument(MODEL, URL, params, data, { idProperty: 'imsi' });
}

export const updateSubscriber = (imsi, params = {}, data = {}) => {
  return updateDocument(MODEL, imsi, `${URL}/${imsi}`, params, data, { idProperty: 'imsi' });
}

export const deleteSubscriber = (imsi, params = {}) => {
  return deleteDocument(MODEL, imsi, `${URL}/${imsi}`, params, { idProperty: 'imsi' });
}
