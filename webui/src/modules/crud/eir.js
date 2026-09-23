import {
  fetchCollection,
  fetchDocument,
  createDocument,
  updateDocument,
  deleteDocument
} from './actions';
export const MODEL = 'eir';
export const URL = '/Eir';
export const fetchEirRecords = (params = {}) =>
  fetchCollection(MODEL, URL, params);
export const fetchEirRecord = (id, params = {}) =>
  fetchDocument(MODEL, id, `${URL}/${id}`, params);
export const createEirRecord = (params = {}, data = {}) =>
  createDocument(MODEL, URL, params, data);
export const updateEirRecord = (id, params = {}, data = {}) =>
  updateDocument(MODEL, id, `${URL}/${id}`, params, data);
export const deleteEirRecord = (id, params = {}) =>
  deleteDocument(MODEL, id, `${URL}/${id}`, params);
