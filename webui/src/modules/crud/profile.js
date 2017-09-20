import {
  fetchCollection,
  fetchDocument,
  createDocument,
  updateDocument,
  deleteDocument
} from './actions'

export const MODEL = 'profiles';
export const URL = '/Profile';

export const fetchProfiles = (params = {}) => {
  return fetchCollection(MODEL, URL, params);
}

export const fetchProfile = (_id, params = {}) => {
  return fetchDocument(MODEL, _id, `${URL}/${_id}`, params);
}

export const createProfile = (params = {}, data = {}) => {
  return createDocument(MODEL, URL, params, data);
}

export const updateProfile = (_id, params = {}, data = {}) => {
  return updateDocument(MODEL, _id, `${URL}/${_id}`, params, data);
}

export const deleteProfile = (_id, params = {}) => {
  return deleteDocument(MODEL, _id, `${URL}/${_id}`, params);
}
