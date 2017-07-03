import { Component } from 'react';
import PropTypes from 'prop-types';

import { Notification } from 'react-notification';

class Snackbar extends Component {
  static propTypes = {
    isActive: PropTypes.bool,
    message: PropTypes.string
  }

  constructor(props) {
    super(props);

    this.state = this.getStateFromProps(props);
  }

  componentWillMount() {
    this.setState(this.getStateFromProps(this.props));
  }

  componentWillReceiveProps(nextProps) {
    console.log(nextProps);
    this.setState(this.getStateFromProps(nextProps));
  }

  getStateFromProps(props) {
    const { 
      isActive
    } = props;

    return {
      isActive
    }
  }

  render() {
    const {
      isActive
    } = this.state;

    const {
      message
    } = this.props;

    return (
      <Notification
        isActive={isActive}
        message={message}
        onDismiss={() => this.setState({ isActive: false })}
        barStyle={{
          left: '50%',
          bottom: '-100%',
          font: '',
          borderRadius: '0px',
          WebkitTransform: 'translate(-50%, 0)',
          MozTransform: 'translate(-50%, 0)',
          msTransform: 'translate(-50%, 0)',
          OTransform: 'translate(-50%, 0)',
          transform: 'translate(-50%, 0)'
        }} 
        activeBarStyle={{
          bottom: '0'
        }} 
      />
    )
  }
}

export default Snackbar;