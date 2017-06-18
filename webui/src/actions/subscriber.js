import {
  fetchCollection, fetchRecord, createRecord, updateRecord, deleteRecord
} from 'redux-crud-store'

const MODEL = 'subscribers'
const PATH = '/Subscriber'

export function fetchSubscribers(params = {}) {
  return fetchCollection(MODEL, PATH, params)
}

export function fetchSubscriber(id, params = {}) {
  return fetchRecord(MODEL, id, `${PATH}/${id}`, params)
}

export function createSubscriber(data = {}) {
  return createRecord(MODEL, PATH, data)
}

export function updateSubscriber(id, data = {}) {
  return updateRecord(MODEL, id, `${PATH}/${id}`, data)
}

export function deleteSubscriber(id) {
  return deleteRecord(MODEL, id, `${PATH}/${id}`)
}