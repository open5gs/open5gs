import React from 'react'
import Header from './header'
import Footer from './footer'

export default class extends React.Component {

  static propTypes() {
    return {
      session: React.PropTypes.object.isRequired,
      children: React.PropTypes.object.isRequired
    }
  }

  render() {
    return (
      <div>
        <Header session={this.props.session}/>
        <div className="container">
          { this.props.children }
        </div>
        <Footer/>
      </div>
    )
  }

}
