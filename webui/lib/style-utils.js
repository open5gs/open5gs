import {css, keyframes} from 'styled-components';

export const media = {
  mobile: (...args) => css`
    @media (max-width: 768px) {
        ${css(...args)}
    }
  `
};

export const transitions = {
slideDown: keyframes`
  0% {
      opacity: 0;
      transform: translateY(-100vh);
  }
  75% {
      opacity: 1;
      transform: translateY(50px);
  }
  100% {
      transform: translateY(0px);
  }
`,
slideUp: keyframes`
  0% {
      transform: translateY(0px);
      opacity: 1;
  }
  25% {
      opacity: 1;
      transform: translateY(50px);
  }
  100% {
      opacity: 0;
      transform: translateY(-100vh);
  }
`,
stretchOut: keyframes`
  0% {
      transform: scale(0,0);
  }
  100% {
      transform: scale(1,1);
  }
`,
shrinkIn: keyframes`
  0% {
      transform: scale(1, 1);
  }
  100% {
      transform: scale(0,0);
  }
`
}