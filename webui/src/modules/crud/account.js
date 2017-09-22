import {
  fetchCollection,
  fetchDocument,
  createDocument,
  updateDocument,
  deleteDocument
} from './actions'

export const MODEL = 'accounts';
export const URL = '/Account';

export const fetchAccounts = (params = {}) => {
  return fetchCollection(MODEL, URL, params, { idProperty: 'username' });
}

export const fetchAccount = (username, params = {}) => {
  return fetchDocument(MODEL, username, `${URL}/${username}`, params, { idProperty: 'username' });
}

export const createAccount = (params = {}, data = {}) => {
  return createDocument(MODEL, URL, params, data, { idProperty: 'username' });
}

export const updateAccount = (username, params = {}, data = {}) => {
  return updateDocument(MODEL, username, `${URL}/${username}`, params, data, { idProperty: 'username' });
}

export const deleteAccount = (username, params = {}) => {
  return deleteDocument(MODEL, username, `${URL}/${username}`, params, { idProperty: 'username' });
}
