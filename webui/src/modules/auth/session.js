/**
 * A class to handle signing in and out and caching session data in sessionStore
 *
 * Note: We use XMLHttpRequest() here rather than fetch because fetch() uses
 * Service Workers and they cannot share cookies with the browser session
 * yet (!) so if we tried to get or pass the CSRF token it would mismatch.
 */

import axios from 'axios';

const authApi = (method, url, csrf, data) => {
  return axios({ 
    baseURL: '/api/auth', 
    headers: { 'X-CSRF-TOKEN': csrf },
    method,
    url,
    data
  });
}

export default class Session {

  constructor({req} = {}) {
    this.session = {}
    try {
      if (req) {
        // If running on server we can access the server side environment
        this.session = {
          csrfToken: req.connection._httpMessage.locals._csrf
        }
        if (req.user) {
          this.session.user = req.user
        }
        if (req.authToken) {
          this.session.authToken = req.authToken;
        }
      } else {
        // If running on client, attempt to load session from localStorage
        this.session = this.getLocalStore('session')
      }
    } catch (err) {
      console.log(err);
    }
  }

  static async getCsrfToken() {
    return new Promise((resolve, reject) => {
      if (typeof window === 'undefined') {
        return reject(Error('This method should only be called on the client'))
      }

      authApi(
        'get', '/csrf'
      ).then(
        response => resolve(response.data.csrfToken)
      ).catch(
        error => reject(Error('Unexpected response when trying to get CSRF token'))
      )
    })
  }

  async getSession(forceUpdate) {
    if (typeof window === 'undefined') {
      return new Promise(resolve => {
        resolve(this.session)
      })
    }

    if (forceUpdate === true) {
      this.session = {}
      this.removeLocalStore('session')
    }

    this.session = this.getLocalStore('session')

    if (this.session && Object.keys(this.session).length > 0 && this.session.expires && this.session.expires > Date.now()) {
      return new Promise(resolve => {
        resolve(this.session)
      })
    }

    return new Promise((resolve, reject) => {
      authApi(
        'get', '/session'
      ).then(
        response => {
          this.session = response.data;
          this.session.expires = Date.now() + this.session.clientMaxAge
          this.saveLocalStore('session', this.session)
          resolve(this.session)
        }
      ).catch(
        error => reject(Error('XMLHttpRequest failed: Unable to get session'))
      )
    })
  }

  async signin(username, password) {
    return new Promise(async (resolve, reject) => {
      if (typeof window === 'undefined') {
        return reject(Error('This method should only be called on the client'))
      }

      const csrf = await Session.getCsrfToken()

      authApi(
        'post', '/login', csrf, { username, password }
      ).then(
        async response => {
          if (response.status !== 200) {
            return reject(Error('XMLHttpRequest error: Unable to login'))
          }
          this.session = await this.getSession(true)
          resolve(true)
        }
      ).catch(
        error => reject(Error('Incorrect username or password.'))
      )
    })
  }

  async signout() {
    return new Promise(async (resolve, reject) => {
      if (typeof window === 'undefined') {
        return reject(Error('This method should only be called on the client'))
      }

      // We aren't checking for success, just completion
      await authApi('post', '/logout', this.session.csrfToken)
      this.session = await this.getSession(true)
      resolve(true)
    })
  }

  // The Web Storage API is widely supported, but not always available (e.g.
  // it can be restricted in private browsing mode, triggering an exception).
  // We handle that silently by just returning null here.
  getLocalStore(name) {
    try {
      return JSON.parse(localStorage.getItem(name))
    } catch (err) {
      return null
    }
  }
  saveLocalStore(name, data) {
    try {
      localStorage.setItem(name, JSON.stringify(data))
      return true
    } catch (err) {
      return false
    }
  }
  removeLocalStore(name) {
    try {
      localStorage.removeItem(name)
      return true
    } catch (err) {
      return false
    }
  }

}
