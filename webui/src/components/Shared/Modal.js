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

  z-index: 10;

  width: ${p => p.width};

  ${media.mobile`
    width: calc(100% - 2rem);
  `}

  .modal-enter {
    animation: ${p => p.transitionEnter};
    animation-fill-mode: forwards;
  }

  .modal-leave {
    animation: ${p => p.transitionLeave};
    animation-fill-mode: forwards;
  }
`;

Wrapper.propTypes = {
  width: PropTypes.string,
  transitionEnter: PropTypes.string,
  transitionLeave: PropTypes.string
};

class Modal extends Component {
  static propTypes = {
    visible: PropTypes.bool,
    onHide: PropTypes.func,
    width: PropTypes.string,
    transitionEnter: PropTypes.string,
    transitionLeave: PropTypes.string
  }

  static defaultProps = {
    width: '400px',
    transitionEnter: `${transitions.stretchOut} .3s ease-in`,
    transitionLeave: `${transitions.shrinkIn} .3s ease-in`
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
      width, 
      transitionEnter,
      transitionLeave 
    } = this.props;

    return (
      <Wrapper 
        width={width} 
        transitionEnter={transitionEnter} 
        transitionLeave={transitionLeave}>
        <CSSTransitionGroup
          transitionName="modal"
          transitionEnterTimeout={500}
          transitionLeaveTimeout={500}>
          {
            visible && (<div>{children}</div>)
          }
        </CSSTransitionGroup>
      </Wrapper>
    );
  }
}

export default onClickOutside(Modal);
