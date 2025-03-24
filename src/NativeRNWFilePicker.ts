import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
  pickFile(types: string[], destPath: string): Promise<Object>;
}

export default TurboModuleRegistry.getEnforcing<Spec>('RnwFilePicker');
