import PropTypes from 'prop-types';
import React, { Component } from 'react';
import styled from 'styled-components';

// ============================

const tooltipArrowHeight = 6;

// const TooltipWrapperButton = styled.button`
//   display: inline-block;
//   outline: none;
//   border: none;
//   background: none;
//   padding: 0;
// `;
const TooltipWrapperDiv = styled.div`
  display: inline-block;
  outline: none;
  border: none;
  background: none;
  padding: 0;
`;
const TooltipContainer = styled.div`
  display: block;
  position: relative;
`;
const TooltipBubble = styled.div`
  position: absolute;
  text-align: center;
  left: ${props => `${props.leftPosition}px`};
  ${props => props.top && `bottom: calc(100% + ${tooltipArrowHeight}px);`};
  ${props => props.bottom && `top: calc(100% + ${tooltipArrowHeight}px);`};
  ${props => (!props.top && !props.bottom) &&
    `bottom: calc(100% + ${tooltipArrowHeight}px);`
  };
  display: inline-block;
  border-radius: 3px;
  min-height: ${props => props.height ? props.height : "30px" };
  min-width: ${props => props.width ? props.width : "100px" };
  padding: ${props => props.padding ? props.padding : "8px" };
  z-index: 999;
  line-height: 1.5;
  font-size: 0.8rem;
  box-sizing: border-box;
  box-shadow: 0px 2px 10px rgba(0,0,0,0.2);
  color: ${props => props.textColor || '#fff'};
  background-color: ${props => props.bgColor || 'rgba(0,0,0,0.8)'};
  border-color: ${props => props.bgColor || 'rgba(0,0,0,0.8)'};
  visibility: ${props => props.show ? 'visible' : 'hidden'};
  ${props => !props.show && 'pointer-events: none;'}
  transition: transform 0.2s ease, opacity 0.3s ease;
  transform: ${props => props.show ?
    'translateY(0px)' :
    `translateY(${props.bottom ? -10 : 10}px)`
  };
  opacity: ${props => props.show ? 1 : 0};
`;

const TooltipArrow = styled.span`
  width: 0px;
  height: 0px;
  border-left: ${`${tooltipArrowHeight}px solid transparent`};
  border-right: ${`${tooltipArrowHeight}px solid transparent`};
  border-top: ${`${tooltipArrowHeight}px solid`};
  border-top-color: inherit;
  position: absolute;
  ${props => props.top && `bottom: -${tooltipArrowHeight}px;`}
  ${props => props.bottom && `top: -${tooltipArrowHeight}px;`}
  ${props => (!props.top && !props.bottom) &&
    `bottom: -${tooltipArrowHeight}px;`
  }
  ${props => props.bottom && 'transform: rotate(180deg);'}
  left: ${`calc(50% - ${tooltipArrowHeight}px)`};
`;

class Tooltip extends Component {
  static propTypes = {
    children: PropTypes.any.isRequired,
    content: PropTypes.any,
    color: PropTypes.string,
    bg: PropTypes.string,
    show: PropTypes.bool,
  }

  static defaultProps = {
    content: 'tooltip content',
    show: null,
  }

  constructor(props) {
    super(props);
    this.showTooltip = this.showTooltip.bind(this);
    this.hideTooltip = this.hideTooltip.bind(this);
    this.handleMouseEnter = this.handleMouseEnter.bind(this);
    this.handleMouseLeave = this.handleMouseLeave.bind(this);

    const disableHoverChanges = props.show != null;

    this.state = {
      showTooltip: false,
      lPos: null,
      disableHoverChanges,
    };
  }

  componentWillReceiveProps(nextProps) {
    // if (nextProps.show !== this.props.show) {
    if (nextProps.show) this.showTooltip();
    else this.hideTooltip();
    // }
  }

  shouldComponentUpdate(nextProps, nextState) {
    const { showTooltip } = this.state;
    return nextState.showTooltip !== showTooltip;
  }

  handleMouseEnter() {
    if (!this.state.disableHoverChanges) this.showTooltip();
  }

  handleMouseLeave() {
    if (!this.state.disableHoverChanges) this.hideTooltip();
  }

  showTooltip() {
    const tipNode = this.tooltipRef;
    const tipWrapNode = this.tooltipWrapRef;
    const lPos = -(tipNode.offsetWidth / 2) + (tipWrapNode.offsetWidth / 2);

    this.setState({ showTooltip: true, lPos });
  }

  hideTooltip() {
    this.setState({ showTooltip: false });
  }

  render() {
    const { content, children, bg, color, ...rest } = this.props;
    const { showTooltip, lPos } = this.state;

    // NOTE: Use `button` wrapper when we need to control tooltip visibility
    // with focus / blur.
    // const Wrapper = disableHoverChanges ?
    //   TooltipWrapperButton :
    //   TooltipWrapperDiv;

    return (
      <TooltipWrapperDiv {...rest} tabIndex='0'>
        <TooltipContainer
          innerRef={(ref) => { this.tooltipWrapRef = ref; }}
          onMouseEnter={this.handleMouseEnter}
          onMouseLeave={this.handleMouseLeave}
        >
          <TooltipBubble
            {...rest}
            innerRef={(ref) => { this.tooltipRef = ref; }}
            leftPosition={lPos || 0}
            show={showTooltip}
            bgColor={bg}
            textColor={color}
          >
            <TooltipArrow {...rest} />
            {content}
          </TooltipBubble>
          {children}
        </TooltipContainer>
      </TooltipWrapperDiv>
    );
  }
}

export default Tooltip;
