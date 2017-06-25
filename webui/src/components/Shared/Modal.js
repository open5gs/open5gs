import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import onClickOutside from 'react-onclickoutside';
import CSSTransitionGroup from 'react-transition-group/CSSTransitionGroup';
import { media, transitions } from 'helpers/style-utils';

const Wrapper = styled.div`
  position: fixed;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);

  z-index: ${p => p.zindex};

  .modal-enter {
    animation: ${p => p.transitionEnter};
    animation-fill-mode: forwards;
  }

  .modal-leave {
    animation: ${p => p.transitionLeave};
    animation-fill-mode: forwards;
  }
`;

class Modal extends Component {
  static propTypes = {
    visible: PropTypes.bool,
    onHide: PropTypes.func,
    zindex: PropTypes.string,
    transitionEnter: PropTypes.string,
    transitionLeave: PropTypes.string,
    transitionEnterTimeout: PropTypes.number,
    transitionLeaveTimeout: PropTypes.number
  }

  static defaultProps = {
    zindex: '10',
    transitionEnter: `${transitions.stretchOut} .25s ease-in`,
    transitionLeave: `${transitions.shrinkIn} .25s ease-in`,
    transitionEnterTimeout: 300,
    transitionLeaveTimeout: 150
  }

  handleClickOutside = (e) => {
    const { visible, onHide } = this.props;

    if (!visible) return null;
    onHide();
  }

  handleKeyUp = (e) => {
    const { onHide } = this.props
    if (e.keyCode === 27) {
      onHide();
    }
  }
  
  componentDidUpdate(prevProps, prevState) {
    if(prevProps.visible !== this.props.visible) {
      if(this.props.visible) {
        document.body.addEventListener('keyup', this.handleKeyPress);
      } else { 
        document.body.removeEventListener('keyup', this.handleKeyPress);
      }
    }
  }
  
  render() {

    const { 
      visible, 
      children, 
      zindex,
      transitionEnter,
      transitionLeave,
      transitionEnterTimeout,
      transitionLeaveTimeout
    } = this.props;

    return (
      <Wrapper 
        zindex={zindex}
        transitionEnter={transitionEnter} 
        transitionLeave={transitionLeave}>
        <CSSTransitionGroup
          transitionName="modal"
          transitionEnterTimeout={transitionEnterTimeout}
          transitionLeaveTimeout={transitionLeaveTimeout}>
          {
            visible && (<div>{children}</div>)
          }
        </CSSTransitionGroup>
      </Wrapper>
    );
  }
}

export default onClickOutside(Modal);
