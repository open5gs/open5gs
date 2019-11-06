import Document, { Head, Main, NextScript } from 'next/document'
import { ServerStyleSheet } from 'styled-components'

const description = 'an implementation of 3GPP EPC(MME, SGW, PGW, HSS)'

export default class MyDocument extends Document {
  render () {
    const sheet = new ServerStyleSheet()
    const main = sheet.collectStyles(<Main />)
    const styleTags = sheet.getStyleElement()
    return (
      <html>
        <Head>
          <title>Open5GS</title>

          <meta charSet="utf-8" />
          <meta httpEquiv="X-UA-Compatible" content="IE=edge,chrome=1" />
          <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no" />

          <meta name="author" content="Open5GS" />
          <meta name="description" content={description} />

          <link rel='icon' href='/static/favicon.ico'/>
          <link rel="stylesheet" type="text/css" href="/static/css/bootstrap.min.css" />
          <link rel="stylesheet" type="text/css" href="/static/css/nprogress.css" />
          <link rel="stylesheet" type="text/css" href="/static/css/index.css" />
          {styleTags}
        </Head>
        <body>
          <div className='root'>
            {main}
          </div>
          <NextScript />
        </body>
      </html>
    )
  }
}
