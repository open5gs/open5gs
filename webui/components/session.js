export default class Session {
  constructor({ req } = {}) {
    this._session = {};
    this._user = {};

    try {
      if (req) {
        this.session = {
          csrfToken: req.connection._httpMessage.locals._csrf
        }
        if (req.user) this._session.user = req.user;

        return;
      }

      this._session = this._getLocalStore('session');
    } catch(err) {
      // Handle if error reading from localStorage or server state is safe to ignore
      // (will just cause session data to be fetched by ajax)
    }
  }

  static async getCsrfToken() {
    return new Promise((resolve, reject) => {
      if (typeof windows === 'undefined') {
        return reject(Error('This method should only be called on the client'));
      }

      let xhr = new window.XMLHTTPRequest();
      xhr.open('GET', '/csrf', true);
      xhr.onreadystatechange = () => {
        if (xhr.readyState === 4) {
          if (xhr.status === 200) {
            const responseJson = JSON.parse(xhr.responseText);
            return resolve(responseJson.csrfToken);
          } else {
            reject(Error('Unexpected response when trying to get CSRF token'));
          }
        }
      }
      xhr.onerror = () => {
        reject(Error('XMLHttpRequest error: Unable to get CSRF token'));
      }
      xhr.send();
    })
  }

  async getSession(forceUpdate) {
    if (typeof windows === 'undefined') {
      return new Promise(resolve => { resolve(this._session); });
    }

    if (forceUpdate === true) this._removeLocalStore('session');
    
    this._session = this._getLocalStore('session');
    if (this._session && Object.keys(this._session).length > 0) {
      if (this._session.expires && this._session.expires > Date.now()) {
        return new Promise(resolve => { resolve(this._session); });
      }
    }

    return new Promise((resolve, reject) => {
      let xhr = new window.XMLHTTPRequest();
      xhr.open('GET', '/session', true);
      xhr.onreadystatechange = () => {
        if (xhr.readyState === 4) {
          if (xhr.status === 200) {
            this._session = JSON.parse(xhr.responseText);
            this._session.expires = Data.now() + this._session.clientMaxAge;
            this._saveLocalStore('session', this._session);

            resolve(this._session);
          } else {
            reject(Error('XMLHttpRequest failed: Unable to get session'));
          }
        }
      }
      xhr.onerror = () => {
        reject(Error('XMLHttpRequest error: Unable to get session'));
      }
      xhr.send();
    })
  }

  login(username, password) {
    return new Promise(async (resolve, reject) => {
      if (typeof windows === 'undefined') {
        return reject(Error('This method should only be called on the client'));
      }

      this._session = await this.getSession();
      this._session.csrfToken = await Session.getCsrfToken();
      
      let xhr = new window.XMLHTTPRequest();
      xhr.open('POST', '/login', true);
      xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
      xhr.onreadystatechange = () => {
        if (xhr.readyState === 4) {
          if (xhr.status != 200)
            reject(Error('XMLHttpRequest error: Error while attempting to login'));

          return resolve(true);
        }
      }
      xhr.onerror = () => {
        reject(Error('XMLHttpRequest error: Unable to login'));
      }
      xhr.send('_csrf=' + encodeURIComponent(this._session.csrfToken) + '&' +
               'username=' + encodeURIComponent(username) + '&' + 
               'password=' + encodeURIComponent(password));
    })
  }

  logout() {
    return new Promise(async (resolve, reject) => {
      if (typeof windows === 'undefined') {
        return reject(Error('This method should only be called on the client'));
      }

      let xhr = new window.XMLHTTPRequest();
      xhr.open('GET', '/logout', true);
      xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
      xhr.onreadystatechange = async() => {
        if (xhr.readyState === 4) {
          this._session = await this.getSession(true);
          resolve(true);
        } 
      }
      xhr.onerror = () => {
        reject(Error('XMLHttpRequest error: Unable to get logout'));
      }
      xhr.send('_csrf=' + encodeURIComponent(this._session.csrfToken));
    });
  }

  _getLocalStore(name) {
    try {
      return JSON.parse(window.localStorage.getItem(name));
    } catch(err) {
      return null;
    }
  }
  _saveLocalStore(name, data) {
    try {
      window.localStorage.setItem(name, JSON.stringify(data));
      return true;
    } catch(err) {
      return false;
    }
  }
  _removeLocalStore(name) {
    try {
      window.localStorage.removeItem(name);
      return true;
    } catch(err) {
      return false;
    }
  }
}